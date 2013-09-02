#include "config.h"
#ifdef USE_AMP
#include <amp.h>
#endif
#include <vector>
using namespace std;
using namespace concurrency;
namespace ann{
	void normalize(vector<float>& _what)
	{
		float sum=0;
		// devide and conq. approach for the below part.....
		for(size_t i(0);i<_what.size();i++)
		{
			sum+= _what[i]*_what[i];
		}
		auto div = sqrtf(sum);
#ifdef USE_AMP
		array_view<float,1> _w(_what.size(),_what.data());
		parallel_for_each(_w.extent,[=](index<1> idx) restrict(amp) {
			_w[idx]/=div;
		});
		_w.synchronize();
#else
		for(size_t i(0);i<_what.size();i++)
		{
			_what[i]/=div;
		}
#endif

	}
	void normalize(FeatureSet<float>& _what)
	{
		float sum=0;
		// devide and conq. approach for the below part.....
		for(size_t i(0);i<_what.size();i++)
		{
			sum+= _what[i]*_what[i];
		}
		auto div = sqrtf(sum);
#ifdef USE_AMP
		array_view<float,1> _w(_what.size(),_what.data());
		parallel_for_each(_w.extent,[=](index<1> idx) restrict(amp) {
			_w[idx]/=div;
		});
		_w.synchronize();
#else
		for(size_t i(0);i<_what.size();i++)
		{
			_what[i]/=div;
		}
#endif

	}

}