#pragma once
#include <vector>
#include <functional>
using namespace std;
template <typename T> class Feature : vector<T>
{
	Feature()
	{
	}
	Feature(const Feature<T>& )
	{
	}
};

