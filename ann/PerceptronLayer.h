// TODO: final implementation and cleanUp
// TODO: implementation of proper destructor
// TODO: deserilizer,

#pragma once
#include <vector>
#include "Perceptron.h"
#include "FeatureSet.h"
#include "unit.h"
#include <map>
#include <utility>
#include <functional>
#include <future>
#include <thread>
#include <stack>
#include <set>
#include <ctime>
#include <atomic>
#include <iostream>
#include <string>
#include "except.h"
#include "config.h"
#include "utility.h"
#include "rapidXml\rapidxml.hpp"
#include "rapidXml\rapidxml_print.hpp"
#ifdef USE_AMP
#include <amp.h>
#include <amp_math.h>
using namespace concurrency;
#endif
typedef pair<int, future<void> > tEntry;
using namespace std;
using namespace rapidxml;
using namespace ann;

namespace ann{
/** this class is a collection of units. it takes FeatureSet<T> as input and generates another FeatureSet<T> as output.
	it is smart enough to be connected to other layers and to generate result at the last connected layer
*/
template<typename T> class PerceptronLayer
{
public:
	PerceptronLayer()
	{
		feedCount = 0;
		/*
		manip = [](vector<FeatureSet<T> > v){
			FeatureSet<T> temp;
			for (int i = 0; i < v.size(); i++)
			{
				temp.combine(v.at(i));
			}
			return temp;
		};
		*/
		inThread.store(NULL);
		ready.store(false);
		clearCount = 0;
		this->_manip = nullptr;
		this->id = -1;	
		subscriber = nullptr;
	}
	/**
	this function will create a perceptronlayer from a example unit and the no of that unit
	@param uni the reference (sample) unit
	@param _count the number of that unit in the perticular layer
	*/
	PerceptronLayer(unit<T> uni, size_t _count) : act(uni.activation()), unitDim(uni.getDim())
	{
		subscriber = nullptr;
		//this->unitDim = uni.getDim();
		this->unitCount = _count;
		this->inpDim = unitDim*unitCount;
		this->outDim = unitCount;
		feedCount = 0;
		inThread.store(NULL);
		ready.store(false);
		clearCount = 0;
		this->_manip = nullptr;
		this->_outChannel = NULL;
		this->_weight.assign(inpDim,1);
		this->id = -1;
		this->_serFlag = false;
		this->output.assign(unitDim, 0);
		//this->inNet = nullptr;
		
	}


	/** this function connects the current layer to another layer. There are two conditions:
		a) the destination layer should constructed with the size_t argument constructor
		b) if the destination is initialised with the 2 argument constructor then des.inpDim == this->outDim and 
		des.unitDim == des.inpDim / des.unitCount
		otherwise the two layers cannot be connected
		@params: PerceptronLayer<T>&
	*/
	void connectTo( PerceptronLayer<T>& p)
	{
		p.connectsFrom.push_back(this);
		connectsTo.push_back(&p);
	}
	/** only public feeder. the other one is private (the no-arg one). accepts vector<T> as input
		improvement: if the argument is taken by referance
	*/
	void _feed(FeatureSet<T> _inp)
	{
#ifdef CONSOLE_DEBUG
		ANN_LOG("layer " + to_string(this->id) + " is fed " + to_string(_inp.size()) + " no of input", this->global_console_lock)
		ANN_LOG("starting calculation", this->global_console_lock)
#endif
		this->input = _inp;
#ifdef CONSOLE_DEBUG
		cout<<"first feed channel "<< this->input.getChannel()<<"\n";
#endif
		this->calculate();
		this->propagate();
	}


	void setInthread(vector<int>::iterator it)
	{
		this->inThread = it;
	}

	void setValid()
	{
		this->valid.store(true);
	}
	void setInvalid()
	{
		this->valid = false;
	}
	bool isValid()
	{
		return valid.load();
	}
	/** this function is called in the accumulation step
	*/
	void clear()
	{
		//this->inThread = NULL;
		clearCount++;
		
		if(clearCount.load()==this->connectsTo.size())
		{
			//this->input.clear();
			this->output.clear();
			this->clearCount.store(0);
			this->ready.store(false);
		}
	}
	/** this function in called in network object to find if the layer is ready
	*/
	bool isReady() const
	{
		return this->ready.load();
	}
	/**
	function to set _manip
	*/
	void customize( function< FeatureSet<T> (FeatureSet<T>) > f)
	{
		this->_manip = (function< FeatureSet<T> (FeatureSet<T>) >*)(new function< FeatureSet<T> (FeatureSet<T>) >(f));
	}

	/**
	this function will be used to change the channel of output, if it is different than output
	*/
	void setOutputChannel(size_t s)
	{
		this->_outChannel = s;
	}
	/**
	this function should be used when a brunchless neural network is being designed without network object
	 review (5.9.2013 1:11pm) why only in case of brunchless network it should be use
	 new use in network id setting
	*/
	void setid(int _id)
	{
		this->id = _id;
	}
	/**implicit conversion operator
	*/
	void operator= (PerceptronLayer<int> _with)
	{

	}
	FeatureSet<T>& weight()
	{
		return this->_weight;
	}

	void _setIdwrtThis()
	{
#ifdef CONSOLE_DEBUG
		ANN_LOG("***layer identifying started***", this->global_console_lock)
#endif
		size_t cId(0);
		stack<PerceptronLayer<T>* > idstck;
		setid(cId++);
		for(size_t i(0);i<connectsTo.size();i++)
		{
			idstck.push(connectsTo[i]);
		}
		while (true)
		{
			
			if(idstck.empty())
				break;
			auto tmp = idstck.top();
			idstck.pop();
			if(tmp->id==-1)
				tmp->setid(cId++);
			for(size_t i(0);i<tmp->connectsTo.size();i++)
			{
				idstck.push(tmp->connectsTo[i]);
			}
		}
#ifdef CONSOLE_DEBUG
		ANN_LOG( to_string(cId) + " layer id'ed", this->global_console_lock)
#endif
	}
	/**
	this function do the serilization of all connected layers wrt the current layer....
	*/
	//xml_document<>* serilizeWrtThis()
	//{
	//	xml_document<>* _ret= new xml_document<>();
	//	xml_node<>* __lyrNode = _ret->allocate_node(node_element,"layer");
	//	_cache.push_back(new string(to_string(this->id)));
	//	size_t _idIdx = _cache.size()-1;
	//	xml_attribute<>* __idAttr = _ret->allocate_attribute("id",this->_cache.at(_idIdx)->c_str());
	//	__lyrNode->append_attribute(__idAttr);
	//	xml_attribute<>* __actAttribute = _ret->allocate_attribute("activation","abcd");
	//	__lyrNode->append_attribute(__actAttribute);
	//	xml_attribute<>* __dim = _ret->allocate_attribute("dim","pqrs");
	//	__lyrNode->append_attribute(__dim);
	//	_cache.push_back(new string());
	//	size_t _wetIdx = _cache.size()-1;
	//	if(this->_weight.size()!=0)
	//	{
	//		_cache.at(_wetIdx)->append(to_string(this->_weight[0]));
	//	}
	//	for(size_t i(1);i<this->_weight.size();i++)
	//	{
	//		_cache.at(_wetIdx)->append(",");
	//		_cache.at(_wetIdx)->append(to_string(this->_weight[i]));
	//	}
	//	xml_attribute<>* _s_weight = _ret->allocate_attribute("weight",_cache.at(_wetIdx)->c_str());
	//	__lyrNode->append_attribute(_s_weight);
	//	_ret->append_node(__lyrNode);
	//	stack<PerceptronLayer<T>*> serStck;
	//	for(size_t i(0);i<connectsTo.size();i++)
	//	{
	//		serStck.push(connectsTo[i]);
	//	}
	//	while (true)
	//	{
	//		if(serStck.empty())
	//		{
	//			this->serilizationComplete();
	//			break;
	//		}
	//		auto _crntLayer = serStck.top();
	//		serStck.pop();
	//		for(size_t i(0);i<_crntLayer->connectsTo.size();i++)
	//		{
	//			serStck.push(_crntLayer->connectsTo[i]);
	//		}
	//		if(!_crntLayer->isSerilized())
	//		{
	//			
	//			xml_node<>* _lyrNode = _ret->allocate_node(node_element,"layer");
	//			_crntLayer->_cache.push_back(new string(to_string(_crntLayer->id)));
	//			size_t __idIdx = _crntLayer->_cache.size()-1;
	//			xml_attribute<>* _idAttr = _ret->allocate_attribute("id",_crntLayer->_cache.at(__idIdx)->c_str());
	//			_lyrNode->append_attribute(_idAttr);
	//			xml_attribute<>* _actAttribute = _ret->allocate_attribute("activation"/*,value here*/);
	//			_lyrNode->append_attribute(_actAttribute);
	//			xml_attribute<>* _dim = _ret->allocate_attribute("dim"/*, value here*/);
	//			_lyrNode->append_attribute(_dim);
	//			_crntLayer->_cache.push_back(new string());
	//			size_t __wetIdx = _crntLayer->_cache.size()-1;
	//			if(_crntLayer->_weight.size()!=0)
	//			{
	//				_crntLayer->_cache.at(__wetIdx)->append(to_string(_crntLayer->_weight[0]));
	//			}
	//			for(size_t i(1);i<_crntLayer->_weight.size();i++)
	//			{
	//				_crntLayer->_cache.at(__wetIdx)->append(",");
	//				_crntLayer->_cache.at(__wetIdx)->append(to_string(_crntLayer->_weight[i]));
	//			}
	//			xml_attribute<>* s_weight = _ret->allocate_attribute("weight",_crntLayer->_cache.at(__wetIdx)->c_str());
	//			_lyrNode->append_attribute(s_weight);
	//			_ret->append_node(_lyrNode);
	//			_crntLayer->serilized();
	//		}
	//	}
	//	return _ret;
	//}
	/**
	function to deserilize the network wrt this network
	*/
	/*void deserilizeWrtThis(xml_document<>* doc)
	{
	}*/
	/**
	function to clear serilization cache..... required to be called by network....
	*/
	void cacheCleanUp()
	{
		stack<PerceptronLayer<T>*> btrack;
		btrack.push(this);
		while (true)
		{
			if (btrack.empty())
			{
				break;
			}
			auto tmp = btrack.top();
			btrack.pop();
			tmp->_cache.clear();
			for (int i = 0; i < tmp->connectsTo.size(); i++)
			{
				btrack.push(tmp->connectsTo[i]);
			}
		}
	}

	void subscribe(vector<T>* func)
	{
		subscriber = func;
	}

	/**
	function to cleanup future containers
	*/
	void threadCleanUp()
	{
		for(int i(0);i<_futBeen.size();i++)
		{
			try{
			//delete _futBeen.at(i);
			}
			catch(exception e)
			{
#ifdef CONSOLE_DEBUG
				cout<<e.what()<<"\n";
#endif
			}
		}
		_futBeen.clear();
	}
	// purpose of the below functions are unspecified
	FeatureSet<T>* getout()
	{
		return new FeatureSet<T>(output);
	}
	void setNetwork(void* _in)
	{
		this->inNet = _in;
	}
	bool ReadyFlag()
	{
		return _rdyFlag.load();	
	}
	PerceptronLayer<T>* data()
	{
		return this;
	}
	T tmpchk()
	{
		return output[0];
	}
	size_t tmpoch()
	{
		return this->output.getChannel();
	}
	FeatureSet<T>& getO()
	{
		return output;
	}
	vector<T>* subscriber;
	int getId() const
	{
		return this->id;
	}
private:
	mutex consLock;
	/** dimension of each unit*/
	const size_t unitDim;
	/** number of unit */
	size_t unitCount;
	/** total input dimension*/
	size_t inpDim;
	/* total output dimension*/
	size_t outDim;
	/** basically useless */
	vector<Perceptron<T> > members;
	/** holds the the id of the current layer */
	int id;
	// needed to be modified
	/** list of from link */
	vector<PerceptronLayer<T>* > connectsTo;
	/** list of to links */
	vector<PerceptronLayer<T>* > connectsFrom;
	/** in the way of removal */
	function< FeatureSet<T> (vector< FeatureSet<T> >) > manip;
	/** current output set of current layer*/
	FeatureSet<T> output;
	/** current input set of current layer */
	FeatureSet<T> input;
	function< Feature<T> (Feature<T> ) > op;
	size_t unit;
	//vector<int>::iterator inThread;
	/** valid flag*/
	atomic<bool> valid;
	//unsigned int inThread;
	/** the layer is in thread this. Custom id. Atomic */
	atomic<int> inThread;
	//static vector< future<void> > threadPool;
	/** static map to mentain threadPool */
	static map<int, future<void>* > threadPool;
	/** mutex for threadPool*/
	mutex threadPoolMutex;
	atomic<bool> _rdyFlag;
	/** variable to set ready flag*/
#ifdef USE_AMP
	/** mutex to get gpu( :D) exclusively */
	mutex ampMutex;
#endif

	size_t feedCount;
	/** ready flag*/
	atomic<bool> ready;
	atomic<int> clearCount;
	/** this manip will hold a function<> object. Which will take the current input as argument (by ref)
		our job is to create the actual input from formal input
	*/
	void* _manip;
	/** variable to hold the formal input (not the actual input)
	*/
	FeatureSet<T> _forInput;
	/**
	variable to hold markar for outpur channel;
	*/
	size_t _outChannel;
	/**
	variable to hold the operation to be performed
	*/
	const _act act;
	/**variable to hold the current weight
	*/
	FeatureSet<T> _weight;
	/**
	flag to hold the serilization state
	*/
	bool _serFlag;
	/**
	variable to keep the serilization variable alive
	*/
	vector<string*> _cache;
	/**
	container to hold future wasteBeen
	*/
	static vector<future<void>* > _futBeen; 


	static mutex global_console_lock;


	
	void operator() (unsigned int ID)
	{
		this->id = ID;
	}
	void ProcessOutput()
	{
		vector<T> temp;
		for (int i = 0; i < members.size(); i++)
		{
			temp.push_back(members.at(i).getOut());
		}
		Feature<T> tfet(temp);
		this->output.clear();
		this->output.addFeature(tfet);
	}



	/** this function checks if all the dependency of the current layer is ready, if not this function makes them ready.
		it uses a depth-first search method to prepair all the dependency
	*/
	void checkDep()
	{
		stack< PerceptronLayer<T>* > bTrack;
		for(int i(0);i<connectsFrom.size();i++)
		{
			bTrack.push(connectsFrom.at(i));
		}
		while (true)
		{
			if(bTrack.empty())		// this should be placed at the start of the loop....// NOW OK!
				break;
			auto temp = bTrack.top();
			bTrack.pop();
			if((temp->inThread.load())!=NULL)
			{
				{
						lock_guard<mutex> thrdPoollockG(threadPoolMutex);
						threadPool.at(temp->inThread.load())->wait();
						
				}
			}
			else
			{
				if(!temp->isReady())		// is it need to be atomic????
				{
					bTrack.push(temp);
					for(int i(0);i<temp->connectsFrom.size();i++)		// this is temp->connectFrom or only connectFrom()
					{
						if (!temp->connectsFrom.at(i)->isReady())
						{
							bTrack.push(temp->connectsFrom.at(i));
						}
					}
				}
				else
				{
					continue;
				}
			}

		}
	}
	/** when all the dependencies of the current layer is ready this function is called. It basically
		consumes the outputs of the previous layers and create the input FeatureSet for the current layer.
		This function consumes a threadTable entry
	*/
	inline void accumulate()
	{
		this->_forInput.clear();
		//size_t _inpChanD = NULL;
		if(connectsFrom.size()!=0)
		{
			this->_forInput.setChannel(connectsFrom[0]->output.getChannel());
		}

		for(int i(0);i<connectsFrom.size();i++)
		{

			this->_forInput.insert(_forInput.begin(),connectsFrom.at(i)->output.begin(),connectsFrom.at(i)->output.end());
			if(connectsFrom.size()!=0) // in ==0 case this function won't be called :D
				_forInput.setChannel(connectsFrom[0]->output.getChannel());
#if DEBUG_LEVEL >0
			if(connectsFrom.at(i)->output.getChannel()!=this->_forInput.getChannel())
			{
				// throw some error or take a log
			}
#endif
			{
				connectsFrom.at(i)->clear();
				lock_guard<mutex> lgMutex(threadPoolMutex);
				auto tmppp= connectsFrom.at(i)->inThread.load() ;
				try{
					if(connectsFrom.at(i)->inThread.load()!=NULL)
					{
						if(threadPool.find(tmppp)!=threadPool.end())
						{
							auto locRef = threadPool.at(tmppp);
							if(locRef->valid())
							{
								locRef->get() ;
							}
							_futBeen.push_back(locRef);
							threadPool.erase(tmppp);
						}
						
						/*
						if(locRef->valid())
						{
							locRef->get() ;
						}
						*/
						//_futBeen.push_back(locRef);
						
							
					}
					else
					{
						// throw some error	
					}
				}
				catch(exception e)
				{
#ifdef CONSOLE_DEBUG
					consLock.lock();
					cout<<e.what()<<"\n";
					consLock.unlock();
#endif
				}
			}
			connectsFrom.at(i)->feedCount = 0;
		}

		if(this->_manip==nullptr)
		{
			this->input = this->_forInput;
		}
		else
		{
			
			this->input = (*((function<FeatureSet<T> ( FeatureSet<T>) >*)_manip))(this->_forInput);
		}

	}
	/** this function creates the output of the current layer */
	void calculate()
	{
#ifdef USE_AMP
		//input.assign(inpDim,0);
		{
#ifdef CONSOLE_DEBUG
			ANN_LOG("layer: " + to_string( this->id) + " waiting for exclusive access to gpu", this->global_console_lock);
#endif
			lock_guard<mutex> gpu(ampMutex);
#ifdef CONSOLE_DEBUG
			ANN_LOG("layer: " + to_string(this->id) + " have gpu", this->global_console_lock)
#endif
			output.assign(outDim,0);
			array_view<T,1> inp(input.size(),input);
			array_view<T,1> out(output.size(),output);
			array_view<T,1> wght(_weight.size(),_weight);
			int dimn = this->unitDim;
			try{
#ifdef CONSOLE_DEBUG
				ANN_LOG("layer " + to_string(this->id) + " feeding <" + to_string(input.size()) + "> dimenssion input vector", this->global_console_lock)
					ANN_LOG("expected unit dim: " + to_string(this->unitDim) +" at layer "+ to_string(this->id), this->global_console_lock)
					ANN_LOG("output vector size before feed:" + to_string(output.size()) + " at layer " + to_string(this->id), this->global_console_lock)
#endif
					parallel_for_each(out.extent, [=](index<1> idx) restrict(amp){
					for (int i = 0; i < dimn; i++)
					{
						out[idx] += inp[(dimn*idx) + i] * wght[(dimn*idx) + i];
					}

				});
			}
			catch (exception e)
			{
				cout<<e.what()<<"\n";
			}
			parallel_for_each(out.extent, [=](index<1> idx) restrict(amp){

				int accuracy = 10;
				float sum = 1.0; // initialize sum of series
				for (int i = accuracy - 1; i > 0; --i)
					sum = 1 + out[idx] * sum / i;
				
				out[idx] = 1 / (1 + (1 / sum));
			});
#ifdef CONSOLE_DEBUG
			ANN_LOG("output vector size after feed: " + to_string(output.size()) + " at layer " + to_string(this->id), this->global_console_lock)
			ANN_LOG("waiting for memory sync at layer: " + to_string(this->id), this->global_console_lock)
#endif
			out.synchronize();
#ifdef CONSOLE_DEBUG
			ANN_LOG("memory sync complete at layer " + to_string(this->id) + " output size:" + to_string(this->id), this->global_console_lock);
#endif
			/*for (size_t i = 0; i <output.size(); i++)
			{
				out[i] = 1 / (1 + (1 / epowX(i)));
			}*/
			this->ready.store(true);
			if (subscriber != nullptr)
			{
				subscriber->clear();
				for (size_t i = 0; i < output.size(); i++)
				{
					subscriber->push_back(output.at(i));
				}
			}
		}
#ifdef CONSOLE_DEBUG
		cout<<this->input.getChannel()<<"\n";
#endif
		if(this->_outChannel!=NULL)
		{
#if DEBUG_LEVEL >1
			if(this->output.size()%this->_outChannel!=0)
			{
				// throw some error or take a log
			}
			else
			{
				this->output.setChannel(this->_outChannel);
			}
#else
			this->output.setChannel(this->_outChannel);
#endif

		}
		else
		{
			this->output.setChannel(this->input.getChannel());
		}
		
		
#if DEBUG_LEVEL > 1
		/*
		if(this->output.size()%this->_outChannel!=0)
		{
			// throw some error
		}
		else
		{
			this->output.setChannel(this->input.getChannel());
		}
		*/
			
#endif
#elif
		//non amp code
#endif
	}
	/** this function is called after calculate function. it checks for the entries in connectTo vector and call feed()
		asynchronusly for all of them and create the thread table entries
	*/
	float epowX(double x)
	{
		int accuracy = 10;
		float sum = 1.0; // initialize sum of series

		for (int i = accuracy - 1; i > 0; --i)
			sum = 1 + x * sum / i;

		return sum;
	}
	inline void propagate()
	{
		
		//this->ready = true;
		for(int i(0);i<connectsTo.size();i++)
		{
			auto temp = generateTid();
			auto p = new future<void>(async(&PerceptronLayer<T>::feed,connectsTo.at(i)));
			{
				lock_guard<mutex> lgtpMutex(threadPoolMutex);
				try{
				threadPool.insert(make_pair(temp,p));
				}
				catch(exception e)
				{
#ifdef CONSOLE_DEBUG
					consLock.lock();
					cout<<e.what()<<"\n";
					consLock.unlock();
#endif
				}
			}
			connectsTo.at(i)->inThread.store(temp);
			
		}
	}
	/** this function generates a unique (to the threadTable) id for each so that they can be retrived later.
		This is just a helper function
	*/
	int generateTid()
	{
		
		srand(time(0));
		auto ret = rand();
		{
			lock_guard<mutex> tplg(threadPoolMutex);
			while(threadPool.find(ret)!=threadPool.end())	// check what find returns... it may be iterator
			{
				ret+=1;
			}
		}
		return ret;
		
	}
	
	/** this function should be private
	this feed will be simple....
	a) check for the no of times called if 0 proceed otherwise return; timely reset counter(don't do it in this feed()
	rather do it in next accumulate)
	b) check for dependency... accumulate will do that... and invalidates the previous layers
	c) create the current input set
	d) feed the this->input to current processing unit
	e) cal feed() for every unit in connectsTo vector
	*/
	void feed()
	{
		
		if ((feedCount++) !=0)
		{
			// i've to remove threads.... guess it's not mandetory
			return;
		}
		//input = manip(accumulate());
		this->checkDep();
		this->accumulate();
		this->calculate();
		if(connectsTo.size()!=0)
		{
			this->propagate();
			
		}

	}
	/**
	sets the serilization flag true
	*/
	void serilized()
	{
		this->_serFlag = true;
	}
	/**
	returns the current state of the serilization state
	*/
	bool isSerilized()
	{
		return this->_serFlag;
	}
	/**
	sets the serilization flage of whole network false
	*/
	void serilizationComplete()
	{
		this->_serFlag = false;
		stack<PerceptronLayer<T>*> serMarker;
		for(size_t i(0);i<this->connectsTo.size();i++)
		{
			serMarker.push(this->connectsTo[i]);
		}
		while (true)
		{
			if(serMarker.empty())
				break;
			auto _tLayer = serMarker.top();
			serMarker.pop();
			for(size_t i(0);i<_tLayer->connectsTo.size();i++)
			{
				serMarker.push(_tLayer->connectsTo[i]);
			}
			_tLayer->_serFlag = false;
			
		}
	}
	
	
};
mutex PerceptronLayer<int>::global_console_lock;
mutex PerceptronLayer<float>::global_console_lock;
map<int, future<void>* > PerceptronLayer<int>::threadPool;
map<int, future<void>* > PerceptronLayer<float>::threadPool;
vector<future<void>* > PerceptronLayer<int>::_futBeen; 
vector<future<void>* > PerceptronLayer<float>::_futBeen;
}