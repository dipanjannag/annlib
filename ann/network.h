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
	network(PerceptronLayer<T>& _initl) : _start(_initl)
	{
		this->_identify();
	}
	void addEndPoint(PerceptronLayer<T>& _endl)
	{
		//_end.push_back(_endl);
		//_endl.setNetwork((void*) this);
		_end.push_back(_endl.data());
	}
	void makeReady()
	{
		for(int i(0);i<_end.size();i++)
		{
			while (true)
			{
				if (_end.at(i)->isReady())
				{
					break;
				}
				this_thread::yield();
			}
		}
		//_start.threadCleanUp();

	}
	/**
	this function will store the current weight. Means it'll create the file if not exists
	otherwise loads it
	*/
	void init()
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
	}
	/**
	the network feed function
	*/
	void feed(FeatureSet<T> _inp)
	{
		//this feed is developed to support multiple layers
		//_start._feed(_inp);
		_start._feed(_inp);
	}
	void feed(vector<T> _inp)
	{
		FeatureSet<T> tmp(_inp);
#ifdef CONSOLE_DEBUG
		cout<<tmp.getChannel()<<"\n";
#endif
		_start._feed(tmp);
	}
	void feed(vector<T> _inp,size_t _ch)
	{
		FeatureSet<T> tmp(_inp,_ch);
#ifdef CONSOLE_DEBUG
		cout<<tmp.getChannel()<<"\n";
#endif
		_start._feed(tmp);
	}
private:
	PerceptronLayer<T>& _start;
	vector<PerceptronLayer<T>*> _end;
	void _identify()
	{
		_start._setIdwrtThis();
	}
};
}
