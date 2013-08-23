#pragma once
#include <vector>
#include "PerceptronLayer.h"
#include "FeatureSet.h"
#include <atomic>
#include <chrono>
/*
using namespace std;
template<typename T> class network
{
	//friend void PerceptronLayer<T>::setReadyFlag();
public:
	network(PerceptronLayer<T>& _initl) : _start(_initl)
	{
		_count=0;
		_currReady=0;
		_initl.setNetwork((void*) this);
	}
	void addEndPoint(PerceptronLayer<T>& _endl)
	{
		//_end.push_back(_endl);
		//_endl.setNetwork((void*) this);
		_end.push_back(_endl);
		_count++;
	}
	bool ready()
	{
		for(int i(0);i<_end.size();i++)
		{
		}
		*/
		/*
		if(_currReady.load()!=this->_count)
		{
			return false;
		}
		else
		{
			return true;
		}
		*/
/*
	}
	void incReady()
	{
		_currReady++;
	}
private:
	PerceptronLayer<T>& _start;
	vector<PerceptronLayer<T>&> _end;
	atomic<int> _currReady;
	size_t _count;
};

*/