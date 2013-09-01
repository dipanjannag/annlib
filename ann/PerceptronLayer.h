// TODO: final implementation and cleanUp
// TODO: implementation of proper destructor

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
#include "except.h"
#include "config.h"
#ifdef USE_AMP
#include <amp.h>
#include <amp_math.h>
using namespace concurrency;
#endif
typedef pair<int, future<void> > tEntry;
using namespace std;
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
		
	}
	/**
	this function will create a perceptronlayer from a example unit and the no of that unit
	@param uni the reference (sample) unit
	@param _count the number of that unit in the perticular layer
	*/
	PerceptronLayer(unit<T> uni,size_t _count)
	{
		this->unitDim = uni.getDim();
		this->unitCount = _count;
		this->inpDim = unitDim*unitCount;
		this->outDim = unitCount;
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
		this->_outChannel = NULL;
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
		this->input = _inp;
		cout<<"first feed channel "<< this->input.getChannel()<<"\n";
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
	bool isReady()
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
	// perpouse of the below functions are unspecified
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
private:
	mutex consLock;
	/** dimension of each unit*/
	size_t unitDim;
	/** number of unit */
	size_t unitCount;
	/** total input dimension*/
	size_t inpDim;
	/* total output dimension*/
	size_t outDim;
	/** basically useless */
	vector<Perceptron<T> > members;
	/** not implemented yet */
	unsigned int id;
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

			_forInput.insert(_forInput.begin(),connectsFrom.at(i)->output.begin(),connectsFrom.at(i)->output.end());
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
						threadPool.at(tmppp)->get() ;
						delete threadPool.at(tmppp);
						threadPool.erase(tmppp);
					}
					else
					{
						// throw some error	
					}
				}
				catch(exception e)
				{
					consLock.lock();
					cout<<e.what()<<"\n";
					consLock.unlock();
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
			lock_guard<mutex> gpu(ampMutex);
			output.assign(outDim,0);
			array_view<T,1> inp(input.size(),input);
			array_view<T,1> out(output.size(),output);
			int dimn = this->unitDim;

			parallel_for_each(out.extent,[=](index<1> idx) restrict(amp){
				for (int i = 0; i < dimn; i++)
				{
					out[idx]+=inp[(dimn*idx)+i];
				}
			});
			out.synchronize();
		}
		cout<<this->input.getChannel()<<"\n";
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
		this->ready.store(true);
		
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
					consLock.lock();
					cout<<e.what()<<"\n";
					consLock.unlock();
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
	
	
};

map<int, future<void>* > PerceptronLayer<int>::threadPool;
}