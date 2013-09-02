#include "PerceptronLayer.h"
#include "unit.h"
#include <vector>
#include <iostream>
#include <utility>
#include <functional>
using namespace ann;
using namespace std;
namespace ann
{
	typedef pair<FeatureSet<>,size_t> trainingEntry;
template<typename T = float> class linearClassifier{
public:
	/**
	constructor to be called with the dimension of initial data
	*/
	linearClassifier(size_t _noCluster,size_t dim)
	{
		unit<T> u(dim,perceptron);
		for(int i(0);i<_noCluster;i++)
		{
			PerceptronLayer<T>* tmp = (PerceptronLayer<T>*)new PerceptronLayer<T>(u,1);
			tmp->setid(i);
			_layers.push_back(tmp);
		}
	}
	void trnFeed(vector<pair<FeatureSet<T>,size_t> > _trnSet)
	{
		while (true)
		{
			size_t _err=0;
			for(int i(0);i<_trnSet.size();i++)
			{
				for (int j = 0; j < _layers.size(); j++)
				{
					_layers.at(j)->_feed(_trnSet.at(i).first);
				}
				// finds which layer is at maximum excitation
				size_t max = 0;
				for(size_t k(1);k<_layers.size();k++)
				{
					if(_layers.at(k)->tmpchk() > _layers.at(max)->tmpchk())
					{
						max = k;
					}
				}
				if(max!=_trnSet.at(i).second)
				{
					_layers.at(_trnSet.at(i).second)->weight()+=_trnSet.at(i).first;
					cout<<"weight for layer "<<i <<" is modified ";
					_err++;
					// change the corresponding weight
				}
			}
			if(_err==0)
				break;
		}
	}
private:
	vector< PerceptronLayer<T>* > _layers;
};
}