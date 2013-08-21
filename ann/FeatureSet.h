#pragma once
#include "feature.h"
#include "calculation.h"
#include "Iterator.h"
#include <functional>
#include <vector>

/** FeatureSet<T> class: basically a container of many features.
    a perceptron accepts a perceptron layer
*/
template<typename T> class FeatureSet 
{
public:
	class iterator: public _vSetRndit<T>
	{	
	public:
        /** @class: iterator
		
		*/
		iterator(vector<T>& v) : _vSetRndit(v)
		{
		}
		iterator(vector<T>& v,size_t ofst) : _vSetRndit(v,ofst)
		{
		}

        // review here
		/*
		iterator(const inerator& src,size_t ofst) : _vSetRndit()
		{
            this->pos = src.pos+ofst;
		}
		*/
	};
    ////////////////////////////////// constructor section //////////////////////////////////////
	FeatureSet() : _it(sets)
	{
		//this->pos = 0;
	}

	FeatureSet(const FeatureSet<T>& src) : _it(sets)
	{
		this->sets = src.sets;
		//this->container = src.container;
		//this->pos = src.pos;
	}
    FeatureSet<T>& operator= (FeatureSet<T> src)
	{
		//this->container = src.container;
		this->sets = src.sets;
		return src;
	}
    ////////////////////////////////// populator section//////////////////////////////////////////////////
	/**
	ready at its new variation
	*/
	void addFeature( Feature<T>& f)
	{
		//this->container.push_back(f);
		for(int i(0);i<f.getDim();i++)
		{
			sets.push_back(f[i]);
		}
	}

	void addFeature(FeatureSet<T>& f)
	{

	}
	/**
	this function is also ready
	*/
	void addFeature( function< Feature<T>(void) > fet)
	{
		auto f = fet();
		//this->container.push_back(f);
		for(int i(0);i<f.getDim();i++)
		{
			sets.push_back(f[i]);
		}
	}

	void init(size_t s,T value)
	{
		sets.assign(s,value);
	}
	/**
	this function is not ready
	*/
	/*
	void putFeature(Feature<T>& f,size_t idx)
	{
		// there is a bound for index..... that should be put here
		container.insert(container.begin()+idx,f); new
		size_t tidx=0;
		for(int i(0);i<idx;i++)
		{
			tidx += container.at(i).getDim();
		}
		for(int i(0);i<f.getDim();i++)
		{
			sets.insert(sets.begin()+tidx+i,f[i]);
		}
	}
	*/
	/**
	this is also not ready
	*/
	void putFeature( function< Feature<T>(void) > fet,size_t idx)
	{
		// there is a bound for index..... that should be put here
		auto f = fet();
		container.insert(container.begin()+idx,f);
		size_t tidx=0;
		for(int i(0);i<idx;i++)
		{
			tidx += container.at(i).getDim();
		}
		for(int i(0);i<f.getDim();i++)
		{
			sets.insert(sets.begin()+tidx+i,f[i]);
		}
	}
    /////////////////////// the below part defines the code required for iterator handling//////////////
	/*
	iterator& begin()
	{
		return *(new FeatureSet<T>::iterator(sets));
	}
	iterator& end()
	{
		auto temp = *(new iterator(sets));
        return temp;
	}
	*/
	iterator begin()
	{
		iterator tmp(this->sets);
		return tmp;
	}
	iterator end()
	{
		iterator tmp(this->sets,sets.size());
		return tmp;
	}
	T& operator[] (size_t idx)
	{
		return sets[idx];
	}
	T& at(size_t idx)
	{
		if (idx>=sets.size())
		{
			cout<<"error";
			return NULL;
		}
		else
		{
			return sets[idx];
		}
	}



    ///////////////////////////////// section for manipulation function////////////////////////////////
	/**
	from the beginning this is ok
	*/
    vector<T> subset(size_t beg,size_t _count)
	{
		vector<T> temp(sets.begin()+beg,sets.begin()+beg + _count);
		return temp;
	}
	/** 
	this function is also ready
	*/
	void combine(FeatureSet<T> with)
	{
		/*
		for(int i(0);i< with.container.size();i++)
		{
			this->container.push_back(with.container.at(i));
		}
		*/
		this->sets.insert(this->sets.end(),with.sets.begin(),with.sets.end());
	}
    ///////////////////////////////// section for helper function//////////////////////////////////////
    
	vector<T>& getValue()
	{
		return this->sets;
	}

	
	void print()
	{
		cout<<"size of of set is "<<sets.size()<<"\n";
	}
    /** i guess this function is no longer needed*/
    
	// clear is needed in accumulation step
	/**
	ready now
	*/
	void clear()
	{
		//container.clear();
		sets.clear();
		//pos = 0;
	}
	void seek(size_t s)
	{
		//this->pos = s;
	}
	void makeValid()
	{
		valid = true;
	}

	bool isValid()
	{
		return valid;
	}

	void makeInvalid()
	{
		valid = false;
	}
    /** i think this operator is really not needed or may be i need it*/
    T operator[] (int i)
	{
		return sets.at(i);
	}
    
	size_t size()
	{
		return sets.size();
	}

	~FeatureSet(void)
	{
	}
private:
	//vector< Feature<T> > container;
	vector<T> sets;
	//size_t pos;
	bool valid;
	iterator _it;

};
