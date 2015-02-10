#pragma once
#include <vector>
#include "PerceptronLayer.h"
#include "FeatureSet.h"
#include "rapidXml\rapidxml.hpp"
#include "rapidXml\rapidxml_print.hpp"
#include <fstream>
#include <atomic>

using namespace std;
using namespace ann;
using namespace rapidxml;
namespace ann{


	template<typename T> class network
	{
		//friend void PerceptronLayer<T>::setReadyFlag();
	public:
		~network()
		{
			
		}
		/*network(size_t unitCount, size_t unitDimension, size_t noHiddenLayer, size_t noOutput)
		{
			unit<T> start_unit(unitDimension);
			PerceptronLayer<T> startLayer(start_unit, unitCount);

			for (size_t i = 0; i < noHiddenLayer; i++)
			{
				unit<T> hiddenUnit(unitCount);
				PerceptronLayer<T> layer(hiddenUnit, unitCount);
				hiddenLayers.emplace_back(layer);
				if (i == 0)
				{
					startLayer.connectTo(hiddenLayers.at(0));
				}
				if (i>0)
				{
					hiddenLayers.at(i - 1).connectTo(hiddenLayers.at(i));
				}
			}
			unit<T> endUnit(noOutput);
			PerceptronLayer<T> endLayer(endUnit, unitCount);
			if (hiddenLayers.size() == 0)
			{
				startLayer.connectTo(endLayer);
			}
			else
			{
				hiddenLayers.at(hiddenLayers.size() - 1).connectTo(endLayer);
			}
			network(startLayer, endLayer);
		}*/
		network(PerceptronLayer<T>* start, PerceptronLayer<T>* end)
		{
			this->_identify();
			available = true;
			/*_end.push_back(_endl.data());
			_end.push_back(_endl.data());
			_end.at(0) = _end[0];*/
		}
		network()
		{
			available = false;
			this->_startPtr = nullptr;
			this->_endPtr = nullptr;
		}
		void assignStart(PerceptronLayer<T>* start)
		{
			this->_startPtr = start;
			if (this->_startPtr != NULL&& this->_endPtr != NULL)
			{
				available = true;
			}
		}
		void assignEnd(PerceptronLayer<T>* end)
		{
			this->_endPtr = end;
			if (this->_startPtr != NULL&& this->_endPtr != NULL)
			{
				available = true;
			}
		}
		void assignStartEnd(PerceptronLayer<T>* start, PerceptronLayer<T>* end)
		{
			this->_startPtr = start;
			this->_endPtr = end;
			if (this->_startPtr != NULL&& this->_endPtr != NULL)
			{
				available = true;
			}
		}
		PerceptronLayer<T> getEndLayer()
		{
			return *(this->_endPtr);
		}
		volatile void makeReady()
		{
			while (true)
			{
				if (_endPtr->isReady())
				{
					break;
				}
				this_thread::yield();
			}
			//_start.threadCleanUp();

		}
		vector<T>* subscribe()
		{
			vector<T>* cont = new vector<T>();
			_endPtr->subscriber = cont;
			while (cont->size() == 0)
			{
				this_thread::yield();
			}
			return cont;
		}
		/**
		this function will store the current weight. Means it'll create the file if not exists
		otherwise loads it
		*/
		/*void init()
		{
		ifstream _dta("tmpp.netx");
		if(_dta.is_open())
		{
		cout<<"file is available";
		}
		else
		{
		_dta.close();
		xml_document<>* __rootDoc = _start.serilizeWrtThis();
		ofstream _data("tmp.netx");
		_data<<*(__rootDoc);
		_data.close();
		_start.cacheCleanUp();
		}
		}*/
		/**
		the network feed function
		*/
		void feed(FeatureSet<T> _inp)
		{
			//this feed is developed to support multiple layers
			//_start._feed(_inp);
			_startPtr->_feed(_inp);
		}
		void feed(vector<T> _inp)
		{
			FeatureSet<T> tmp(_inp);
#ifdef CONSOLE_DEBUG
			cout << tmp.getChannel() << "\n";
#endif
			_startPtr->_feed(tmp);
		}
		/*void feed(vector<T> _inp, size_t _ch)
		{
			FeatureSet<T> tmp(_inp, _ch);
#ifdef CONSOLE_DEBUG
			cout << tmp.getChannel() << "\n";
#endif
			_start._feed(tmp);
		}*/

		void appendHiddenLayer(PerceptronLayer<T>* layer)
		{
			hiddenLayers.push_back(layer);
		}
		
		void _identify()
		{
			_startPtr->_setIdwrtThis();
		}
	private:
		PerceptronLayer<T>* _startPtr;
		//PerceptronLayer<T> _startLayer;
		PerceptronLayer<T>* _endPtr;
		//PerceptronLayer<T> _endLayer;
		
		bool available = false;
		vector<PerceptronLayer<T>* > hiddenLayers;

	};

	template<typename T> network<T>* create_network(size_t unitCount,
		size_t unitDimension, size_t noHiddenLayer, size_t noNeuron, size_t noOutput, T initWeight = 1.0f)
	{
		network<T>* ret = new network<T>() ;
		unit<T> u(unitDimension);
		PerceptronLayer<T>* p = new PerceptronLayer<T>(u, unitCount);
		ret->assignStart(p);
		PerceptronLayer<T>* lastConnected = p;
		for (size_t i = 0; i < noHiddenLayer; i++)
		{
			unit<T> un(lastConnected->getOutputDimension());
			auto temp = new PerceptronLayer<T>(un, noNeuron);
			p->connectTo(*temp);
			ret->appendHiddenLayer(temp);
			p = temp;
		}
		unit<T> un(p->getOutputDimension());
		PerceptronLayer<T>* lastLayer = new PerceptronLayer<T>(un, noOutput);
		ret->assignEnd(lastLayer);
		return ret;
	}
}
