#pragma once
#include <vector>
#include <functional>
using namespace std;
//using namespace ann;
namespace ann{
template <typename T> class Feature : vector<T>
{
	Feature()
	{
	}
	Feature(const Feature<T>& )
	{
	}
};
}

