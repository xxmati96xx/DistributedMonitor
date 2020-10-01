#define MONITOR_H
#include "../sk/sk.h"
#include <algorithm>

#include <string.h>

using namespace std;
class Monitor{
    public:
        Monitor(string address,bool useToken,vector<string> other);
        ~Monitor();
        void in();
        void out();
        void setData(string data);
        string getData();


    private:
        SK sk;
        void *context;
        void *socket;
        void createSocket();
        void receiveMessage();
    
};