#pragma once
#include <vector>
#include <assert.h>
using namespace std;
// the previous class is needed to be updated to support enhanced functionality
template<typename T> class Perceptron
{
public:
	Perceptron(size_t Dimenssion)
	{
		this->dimension = Dimenssion;
		this->currentInput.reserve(this->dimension);
		this->weight.reserve(this->dimension);
		this->weight.assign(dimension,1.00);
	}
	Perceptron(vector<T> initWeight)
	{
		this->dimension = iniW.size();
		this->currentInput.reserve(this->dimension);
		this->weight = iniW;
	}

	Perceptron(const Perceptron& p)
	{
		this->dimension = p.dimension;
		this->currentInput.reserve(this->dimension);
		this->weight.reserve(this->dimension);
		this->weight.assign(dimension,1.00);
	}
	~Perceptron(void)
	{
	}
	void feed(vector<T> inp)
	{
		if(inp.size()+1!=this->dimension)
		{
			cout<<"incomp Dimenssion\n";
			return;
		}
		this->currentInput = inp;
		this->currentInput.push_back(1);
		this->process();
		//this->currentInput[this->currentInput.capacity()-1] = 1;
	}
	virtual int getOutput()
	{
		if(this->currentOutput>0)
			return 1;
		else
			return 0;
	}
	T getOut()
	{
		return this->currentOutput;
	}
	void changeDimenssion(size_t newDim)
	{
		
		this->currentInput.clear();
		this->weight.clear();
		this->currentInput.reserve(nSize);
		this->weight.reserve(nSize);
	}
	size_t getDim()
	{
		return dimension;
	}
	//void p();
	//virtual void train(vector<vector<int>> trnData);
//private:
	vector<T> currentInput;
	vector<T> weight;
	double currentOutput;
	size_t dimension;
	void process()
	{
		//assert(this->currentInput.capacity()==this->weight.capacity());
		//assert(this->currentInput.capacity()==this->currentInput.size() && this->weight.capacity()==this->weight.size());
		double sum=0;
		for(auto i(0);i<this->dimension;i++)
		{
			sum+=this->currentInput[i]*this->weight[i];	// quite simply this line is throwing exception and very easy why...
		}
		this->currentOutput = sum;

	}
};

