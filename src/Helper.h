#include <map>
#include "Parser.h"

using namespace std;

template <typename T, typename U>
class createMap
{
    private:
        map<T, U> mMap;

    public:
        createMap(const T& key, const U& val)
        {
            mMap[key] = val;
        }

        createMap<T, U>& operator()(const T& key, const U& val)
        {
            mMap[key] = val;
            return *this;
        }

        operator map<T, U>()
        {
            return mMap;
        }
};

class FunctionCaller
{
    public:
        int argsCount;
        union Args_t
        {
            Node&(*arg1)(Node&);
            Node&(*arg2)(Node&, Node&);
        } args;
};

class Arg_1 : public FunctionCaller
{
    public:
        Arg_1(Node&(*fNode)(Node&))
        {
            argsCount = 1;
            args.arg1 = fNode;
        }
};

class Arg_2 : public FunctionCaller
{
    public:
        Arg_2(Node&(*fNode)(Node&, Node&))
        {
            argsCount = 2;
            args.arg2 = fNode;
        }
};
