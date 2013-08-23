#pragma once
#include <vector>
#include <functional>
using namespace std;
template <typename T> class Feature
{
	// this class should provide it's own iterator
public:
	/**

	this function creates a feature array with dimension dim and of initial value def
	*/
	Feature(int dim,T def)
	{
		this->dimension = dim;
		fet = new T[this->dimension];
		for(int i(0);i<this->dimension;i++)
			fet[i]= def;
		this->pos = 0;

	}

	/**
	this function creates a feature array from a vector
	*/
	Feature(vector<T> inp)
	{
		this->dimension = inp.size();
		fet = new T[this->dimension];
		for(int i(0);i<this->dimension;i++)
			fet[i]= inp.at(i);
		this->pos = 0;

	}
	
	Feature(function< vector<T>(void) > func)
	{
		auto input = func();
		fet = new T[input.size()];
		this->dimension = input.size();
		for(int i(0);i<this->dimension;i++)
			fet[i] = input.at(i);
		this->pos =0;
	}
	~Feature(void)
	{

	}

	/**
	this function modifies the value of a feature array
	*/
	void setFeature(vector<T> input)
	{
		//fet = (T*) malloc(input.size()*sizeof(T));
		fet = new T[input.size()];
		cout<<"this is called";
		if(input.size()!= this->dimension)
			return;
		for(int i(0);i<this->dimension;i++)
			fet[i] = input.at(i);
		this->pos = 0;
	}


	void setFeature(function< vector<T>(void) > func)
	{
		auto input = func();
		if(this->dimension!=input.size())
		{
			cout<<"invalid argument";
			return;
		}
		for(int i(0);i<this->dimension;i++)
			fet[i] = input.at(i);
		this->pos =0;
	}


	/**
	returns the dimenssion of current feature
	*/

	int getDim()
	{
		return this->dimension;
	}
	

	/**
	[] operator is overloaded for Feature class
	*/
	T operator[] (int i)
	{
		return fet[i];
	}
	T next()
	{
		return fet[(this->pos)++];
	}
	bool hasNext()
	{
		if(this->pos+1 > this->dimension)
			return false;
		else
			return true;
	}
	void reset()
	{
		this->pos = 0;
	}

	//overloads from iterator<t>
	
private:
	int dimension;
	T* fet;
	size_t pos;
	//size_t size;
};

