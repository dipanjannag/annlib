#include <exception>
#include <string>
using namespace std;
enum errTyp{
	CanNotConnect,
};

class _excpt : exception
{
public:

	_excpt(const char* whatMessage,errTyp e) : exception(whatMessage)
	{
	}
	_excpt(const char* whatMessage) : exception(whatMessage)
	{
	}
	~_excpt()
	{
	}

protected:
	int errCode;

};

#define MAKE_ERROR_REF(type) { switch type :			\
{\
	case CanNotConnect:\
\
	new _excpt("the two layers can not be connected",type)\
	break;													\
}\
\
}