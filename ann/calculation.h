#pragma once
#include <amp.h>
#include <functional>
#include <vector>

using namespace std;
template<typename T> class _calc
{
public:
	_calc(void)
	{
		activation = [](int arg){ return arg;};
	}
	_calc( function<T (T)> act)
	{
		activation = act;
	}
	void addActivation( function<T (T)> act)
	{
		activation = act;
	}
	T operator() (T arg)
	{

	}
	//_calc();
	~_calc(void)
	{
	}

private:
	function< T (T) > activation;
};

