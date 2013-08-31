#pragma once
#include "feature.h"
#include "Iterator.h"
#include "index.h"
#include <functional>
#include <vector>
using namespace ann;
namespace ann{
/** FeatureSet<T> class: basically a container of many features.
    a perceptron accepts a perceptron layer
*/
template<typename T> class FeatureSet :public vector<T>
{
//private:
	const size_t _chan;
public:
	FeatureSet(vector<T>& v,size_t ch = 1) : vector<T>(v) ,_chan(ch)
	{
	}
	FeatureSet() : _chan(1)
	{
	}
	void operator= (const FeatureSet<T>& _to)
	{
		if(this->_chan!=_to._chan)
		{
			cout<<"featureSet can not be initialised";
		}
		else
		{
			clear();
			insert(begin(),_to.begin(),_to.end());
			//return *this;
		}
	}

};
}