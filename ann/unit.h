#pragma once
#include "globals.h"
template<typename T> class unit
{

public:
	unit(size_t d) : dim(d)
	{
	}

	unit(size_t d,_act f)
	{
		this->dim=d;
		this->func = f;
	}
	size_t getDim()
	{
		return this->dim;
	}
	~unit(void)
	{
	}
private:
	_act func;
	const size_t dim;
};
