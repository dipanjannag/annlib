#pragma once
#include "feature.h"
#include "Iterator.h"
#include "index.h"
#include <functional>
#include <vector>
#include <exception>
using namespace ann;
namespace ann{
/** FeatureSet<T> class: basically a container of many features.
    a perceptron accepts a perceptron layer
*/
template<typename T= float> class FeatureSet :public vector<T>
{

public:
	FeatureSet(vector<T> v,size_t ch = 1) : vector<T>(v)
	{
		_chan = ch;
		_dim= v.size()/ch;
	}
	FeatureSet() 
	{
		_chan = 1;
		_dim = NULL;
	}
	FeatureSet(const FeatureSet<T>& _src)
	{
		this->_chan = _src._chan;
		this->_dim = _src._dim;
		clear();
		insert(begin(),_src.begin(),_src.end());
	}
	void setDim(size_t s)
	{
		if(_dim!=NULL)
		{
#if DEBUG_LEVEL > 1
			//throw some error
#else
			this->_dim = s;
#endif
		}
		else
		{
			this->_dim = s;
		}
	}
	void operator= (const FeatureSet<T>& _to)
	{
		clear();
		insert(begin(),_to.begin(),_to.end());
		this->_chan = _to._chan;
		this->_dim = _to._dim;
			//return *this;
	}
	void setChannel(size_t c)
	{
		_chan = c;
	}
	size_t getChannel()
	{
		return _chan;
	}
	size_t getDim()
	{
		return this->_dim;
	}
	/**necessery overloads
	*/
	void operator+= (FeatureSet<T> _op)
	{
		size_t t_size = size();
		size_t oth_size = _op.size();
		if(t_size<=oth_size)
		{
			for(size_t i(0);i<t_size;i++)
			{
				at(i)+=_op[i];
			}
		}
		else
		{
			for(size_t i(0);i<oth_size;i++)
			{
				at(i)+=_op[i];
			}
		}
	}
	void operator-= (FeatureSet<T> _op)
	{
		size_t t_size = size();
		size_t oth_size = _op.size();
		if(t_size<=oth_size)
		{
			for(size_t i(0);i<t_size;i++)
			{
				at(i)-=_op[i];
			}
		}
		else
		{
			for(size_t i(0);i<oth_size;i++)
			{
				at(i)-=_op[i];
			}
		}
	}
private:
		size_t _chan;
		size_t _dim;

};
}