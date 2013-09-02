#include "PerceptronLayer.h"
#include "unit.h"
using namespace ann;
namespace ann
{
template<typename T> class linearClassifier{
public:
	/**
	constructor to be called with the dimension of initial data
	*/
	linearClassifier(size_t _noCl,size_t dim)
	{
		unit<T> u(dim,perceptron);
		for(int i(0);i<_noCl;i++)
		{
			auto tmp = (PerceptronLayer<T>*)new PerceptronLayer(u,1);
			tmp->setid(i);
			_layers.push_back(tmp);
		}
	}
private:
	vector< PerceptronLayer<T>* > _layers;
};
}