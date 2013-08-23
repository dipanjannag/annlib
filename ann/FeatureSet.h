#pragma once
#include "feature.h"
#include "Iterator.h"
#include <functional>
#include <vector>

/** FeatureSet<T> class: basically a container of many features.
    a perceptron accepts a perceptron layer
*/
template<typename T> class FeatureSet :public vector<T>
{
//private:
	const size_t _dim;
public:
	FeatureSet(vector<T>& v,size_t dim = 1) : vector<T>(v) ,_dim(dim)
	{
	}
	FeatureSet() : _dim(1)
	{
	}
	FeatureSet<T>& operator= (const FeatureSet<T>& _to)
	{
		if(this->_dim!=_to._dim)
		{
			cout<<"featureSet can not be initialised";
		}
		else
		{
			insert(end(),_to.begin(),_to.end());
			return *this;
		}
	}

};
