#pragma once
#include <iterator>
#include <vector>
using namespace std;
/**_indIt<T> is a custom random access iterator derived from std::iterator. This class is abstract and ony to inherit from std::iterator
	and to give a prototype how a iterator for ann dataset should be
	
*/
template<typename T> class _rndIt : std::iterator<std::random_access_iterator_tag,T> {	
public:
	_rndIt()
	{
		pos=0;
	}
	_rndIt(size_t s)
	{
		pos=s;
	}
	virtual T& operator++ () = 0;
	virtual T& operator-- () = 0;
	virtual T& operator* () = 0;
	
protected:
	size_t pos;
};
/**_fwdIt<T> is a custom forward iterator derived from std::iterator. This class is abstract and ony to inherit from std::iterator
	and to give a prototype how a iterator for ann dataset should be */
template<typename T> class _fwdIt : std::iterator<std::forward_iterator_tag,T> {	
public:
	_fwdIt()
	{
		pos=0;
	}
	_fwdIt(size_t s)
	{
		pos=s;
	}
	virtual T& operator++ () = 0;
	virtual T operator* () = 0;
protected:
	size_t pos;
};
/**this class inherit from _rndIt<T> and provides the necessery prototype for a vector based dataset.
	this interface provides the following operator
	a) ++/--
	b) *
	Aditionally this class has no default constructor. Only one constructor to be called with the dataset vector as argument,
	as it internally mentains a reference to that.
	@file: iterator.h
*/
template<typename T> class _vSetRndit : _rndIt<T> {
public:
	_vSetRndit(vector<T>& v) : setsRef(v)
	{
		pos = 0;
	}
	virtual T& operator++ ()
	{
		return setsRef[pos++];
	}
	virtual T& operator-- ()
	{
		return setsRef[pos--];
	}
	virtual T& operator* ()
	{
		return setsRef[pos];
	}
protected:
	vector<int>& setsRef;
};