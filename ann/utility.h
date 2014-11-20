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




}