#include "network.h"
template<typename T> class network_manager
{
public:
	network_manager()
	{

	}

	network<T>& get_instance(size_t unitCount,
		size_t unitDimension, size_t noHiddenLayer, size_t noNeuron, size_t noOutput, T initWeight = 1.0f)
	{
		network<T>* ret = new network<T>();
		unit<T> u(unitDimension);
		PerceptronLayer<T>* p = new PerceptronLayer<T>(u, unitCount);
		ret->assignStart(p);
		PerceptronLayer<T>* lastConnected = p;
		for (size_t i = 0; i < noHiddenLayer; i++)
		{
			unit<T> un(lastConnected->getOutputDimension());
			auto temp = new PerceptronLayer<T>(un, noNeuron);
			p->connectTo(*temp);
			ret->appendHiddenLayer(temp);
			p = temp;
		}
		unit<T> un(p->getOutputDimension());
		PerceptronLayer<T>* lastLayer = new PerceptronLayer<T>(un, noOutput);
		p->connectTo(*lastLayer);
		ret->assignEnd(lastLayer);
		instances.push_back(ret);
		ret->_identify();
		return *ret;
	}
	~network_manager()
	{
		/*for (size_t i = 0; i < instances.size(); i++)
		{
			network<T>* target = instances.at(i);
			delete target->_startPtr;
			
			for (size_t j = 0; j < target->hiddenLayers.size(); j++)
			{
				delete target->hiddenLayers.at(i);
			}
			delete target->_endPtr;
		}*/
	}
private:
	vector<network<T>* > instances;
private:

};