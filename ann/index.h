/*
#include <utility>
#include <functional>
using namespace std;

namespace ann{
	class index {
	private:
		const size_t dim;
		int* _store;
	public:
		index(int a) : dim(1)
		{
			_store = new int[1];
			_store[0]=a;
		}
		index(int a,int b) : dim(2)
		{
			_store = new int[2];
			_store[0]=a;
			_store[1]=b;
		}
		index(int a,int b,int c) : dim(3)
		{
			_store = new int[3];
			_store[0]=a;
			_store[1]=b;
			_store[2]=c;
		}
	};
}
*/