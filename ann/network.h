#pragma once
#include <vector>
#include "PerceptronLayer.h"
using namespace std;
/*
template<typename T> class network
{
	network()
	{
	}
	network(PerceptronLayer<T>& pl)
	{
		members.push_back(pl);
	}

	void addLayer( PerceptronLayer<T>& pl)
	{
		members.push_back(pl);
	}

	void putLayer(PerceptronLayer<T>& pl, size_t idx)
	{
		members.insert(idx,pl);
	}
	
	void feed(T inp[])
	{
	}
	void feed(T* inp)
	{
	}
	void feed(vector<T> inp)
	{
	}
	void feed(FeatureSet<T>& inp)
	{
		// this feeding will be nothing but a tree, not binary at least, traversal.......
		for(int i(0);i<this->iniLayer.size();i++)
		{
			iniLayer[i].feed(inp);

		}
	}
	/*
	friend void PerceptronLayer::setId(int Id);
public:
	network(void);
	~network(void);
	
	void add(PerceptronLayer& p);
	void feed();


private:
	vector<PerceptronLayer<T>&> members;
	vector< PerceptronLayer<T>& > iniLayer;
	int currentSize;
	void setId(PerceptronLayer& p);	
};

*/
