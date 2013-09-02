#pragma once
#include "globals.h"
using namespace ann;
namespace ann{
template<typename T> class unit
{

public:
	unit(size_t d) : dim(d)
	{
		this->func = perceptron;
	}

	unit(size_t d,_act f) : dim(d)
	{
		this->func = f;
	}
	size_t getDim()
	{
		return this->dim;
	}
	void setActivation(_act a)
	{
		this->func = a;
	}
	_act activation()
	{
		return this->func;
	}
	~unit(void)
	{
	}
private:
	_act func;
	const size_t dim;
};
}