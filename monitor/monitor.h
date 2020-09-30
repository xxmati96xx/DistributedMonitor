#define MONITOR_H
#include "../sk/sk.h"
#include <algorithm>
#include <thread>
#include <string.h>

using namespace std;
class Monitor{
    public:
        Monitor(int port,bool useToken,vector<int> other);
        ~Monitor();
        void in();
        void out();
        void setData(string data);
        string getData();


    private:
        SK sk;
        void *context;
        void *socket;
        void receiveMessage();
    
};