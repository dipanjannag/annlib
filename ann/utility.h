#include "config.h"
#ifdef USE_AMP
#include <amp.h>
#endif
#include <vector>
#include <string>
using namespace std;
using namespace concurrency;


#define ANN_LOG(what, ConsoleLock)				\
{												\
	lock_guard<mutex> locker(ConsoleLock);		\
	cout << what << "\n";						\
}												\

namespace ann{
	/**
	Example defination language: <5,1000><2,500><5,100><100,1>

	*/


}