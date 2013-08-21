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
typedef pair<int, future<void> > tEntry;
using namespace std;



/** this class is a collection of units. it takes FeatureSet<T> as input and generates another FeatureSet<T> as output.
	it is smart enough to be connected to other layers and to generate result at the last connected layer
*/
template<typename T> class PerceptronLayer
{
public:
	PerceptronLayer()
	{
		//this->unitDim = u.dim;
		feedCount = 0;
		manip = [](vector<FeatureSet<T> > v){
			FeatureSet<T> temp;
			for (int i = 0; i < v.size(); i++)
			{
				temp.combine(v.at(i));
			}
			return temp;
		};
		inThread.store(NULL);
		ready = false;
		clearCount = 0;
		
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
		manip = [](vector<FeatureSet<T> > v){
			FeatureSet<T> temp;
			for (int i = 0; i < v.size(); i++)
			{
				temp.combine(v.at(i));
			}
			return temp;
		};
		inThread.store(NULL);
		ready = false;
		clearCount = 0;
		
	}
	PerceptronLayer(unit<T> uni)
	{
		this->unitDim = uni.getDim();
		//this->unitCount = _count;
		//this->inpDim = unitDim*unitCount;
		//this->outDim = unitCount;
		feedCount = 0;
		manip = [](vector<FeatureSet<T> > v){
			FeatureSet<T> temp;
			for (int i = 0; i < v.size(); i++)
			{
				temp.combine(v.at(i));
			}
			return temp;
		};
		inThread.store(NULL);
		ready = false;
		clearCount = 0;
		
	}
	/**
	this constructor creates the layer with the
	*/
	/*
	PerceptronLayer(size_t unitno)
	{
		this->unitDim = NULL;
		this->unitCount = unitno;
		this->inpDim = NULL;
		this->outDim = unitCount;
		feedCount = 0;
		manip = [](vector<FeatureSet<T> > v){
			FeatureSet<T> temp;
			for (int i = 0; i < v.size(); i++)
			{
				temp.combine(v.at(i));
			}
			return temp;
		};
		inThread.store(NULL);
		ready = false;
		clearCount = 0;
		
	}
	*/
	/** this function connects the current layer to another layer. There are two conditions:
		a) the destination layer should constructed with the size_t argument constructor
		b) if the destination is initialised with the 2 argument constructor then des.inpDim == this->outDim and 
		des.unitDim == des.inpDim / des.unitCount
		otherwise the two layers cannot be connected
		@params: PerceptronLayer<T>&
	*/
	void connectTo( PerceptronLayer<T>& p)
	{
		if(p.inpDim!=NULL)	// what should null
		{
			// throw some error
		}
		else
		{
			p.inpDim = this->outDim;
			p.unitCount = p.inpDim/p.unitDim;
			p.outDim = p.unitCount;
		}
		p.connectsFrom.push_back(this);
		connectsTo.push_back(&p);
	}
	/** only public feeder. the other one is private (the no-arg one). accepts vector<T> as input
		improvement: if the argument is taken by referance
	*/
	void feedv(vector<T> inp)
	{
		Feature<T> temp(inp);
		FeatureSet<T> temp1;
		temp1.addFeature(temp);
		this->input = temp1;
		this->calculate();
		this->propagate();
	}

	void setInthread(vector<int>::iterator it)
	{
		this->inThread = it;
	}

	void setValid()
	{
		this->valid = true;
	}
	void setInvalid()
	{
		this->valid = false;
	}
	bool isValid()
	{
		return valid;
	}
	void clear()
	{
		//this->inThread = NULL;
		clearCount++;
		
		if(clearCount=this->connectsTo.size())
		{
			this->inThread.store(NULL);
			this->input.clear();
			//this->output.clear();
			this->clearCount=0;
		}
	}
	bool isReady()
	{
		return ready;
	}
	FeatureSet<T>* getout()
	{
		return new FeatureSet<T>(output);
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
	bool valid;
	//unsigned int inThread;
	/** the layer is in thread this. Custom id. Atomic */
	atomic<int> inThread;
	//static vector< future<void> > threadPool;
	/** static map to mentain threadPool */
	static map<int, future<void>* > threadPool;
	/** mutex for threadPool*/
	mutex threadPoolMutex;

	size_t feedCount;
	/** ready flag*/
	bool ready;
	atomic<int> clearCount;
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
				/*
				threadPoolMutex.lock();
				try{
				threadPool.at(temp->inThread.load())->wait();
				// 18/8/2013 reported error invalid map key
				threadPoolMutex.unlock();
				}
				catch( exception e){
					cout<<e.what()<<"\n";
					threadPoolMutex.unlock();
				}
				*/
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
		this->input.clear();	//defined in FeatureSet	
		
		for(int i(0);i<connectsFrom.size();i++)
		{
			// put a review here
			input.combine(connectsFrom.at(i)->output);
			//connectsFrom.at(i)->clear();	// make sure this make is available
			// here to acquire lock
			//if(threadPool[connectsFrom.at(i)->inThread]->valid())
				//threadPool[connectsFrom.at(i)->inThread]->get();
			{
				lock_guard<mutex> lgMutex(threadPoolMutex);
				auto tmppp= connectsFrom.at(i)->inThread.load() ;
			//threadPoolMutex.lock();
				try{
					if(connectsFrom.at(i)->inThread.load()!=NULL)
					{
						cout<<"fuck me up\n";
						/*
						consLock.lock();
						cout<<"trying to erase entry\n";
						consLock.unlock();
						*/
						delete threadPool.at(tmppp);
						threadPool.erase(tmppp);

						consLock.lock();
						//cout<<"entry erased\n";
						cout<<"thread id " <<tmppp <<"deleted\n";
						consLock.unlock();
					}
					else
					{
						consLock.lock();
						cout<<" the specified key is null\n";
						consLock.unlock();
						
					}
					// this erase is causing the whole thing
				}
				catch(exception e)
				{
					consLock.lock();
					cout<<e.what()<<"\n";
					consLock.unlock();
				}
				
				//threadPoolMutex.unlock();
			}
			connectsFrom.at(i)->feedCount = 0;
			//this->setValid(); //may not be here though
		}

	}
	/** this function creates the output of the current layer */
	void calculate()
	{
		this->output.clear();
		this->output = this->input;
		this->ready = true;
		/*
		if(connectsTo.size()!=0)
		{
			while(this->output.hasNext())
			{
				cout<<output.next()<<"\t";
			}
		}
		*/
	}
	/** this function is called after calculate function. it checks for the entries in connectTo vector and call feed()
		asynchronusly for all of them and create the thread table entries
	*/

	inline void propagate()
	{
		for(int i(0);i<connectsTo.size();i++)
		{
			auto temp = generateTid();
			//auto chk = connectsTo.at(i)->feed;
			auto p = new future<void>(async(&PerceptronLayer<T>::feed,connectsTo.at(i)));
			//auto p = new future<void>(async(&PerceptronLayer<T>::feed,connectsTo.at(i)));
			//threadPoolMutex.lock();
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
			consLock.lock();
			cout<<"thread id " <<temp<<" stored\n";
			consLock.unlock();
			//cout<<"temp value "<<connectsTo.at(i)->inThread.load()<<"\n";
			
		}
	}
	/** this function generates a unique (to the threadTable) id for each so that they can be retrived later.
		This is just a helper function
	*/
	int generateTid()
	{
		
		srand(time(0));
		auto ret = rand();
		//threadPoolMutex.lock();
		{
			lock_guard<mutex> tplg(threadPoolMutex);
			while(threadPool.find(ret)!=threadPool.end())	// check what find returns... it may be iterator
			{
				ret+=1;
			}
		}
		//threadPoolMutex.unlock();
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
		if(this->connectsTo.size()!=0)
		{
			this->propagate();
		}

	}
	
};
//std::map<T,T2> YourClass::YourMember = std::map<T,T2>();
map<int, future<void>* > PerceptronLayer<int>::threadPool; //= map<int, future<void>* >();