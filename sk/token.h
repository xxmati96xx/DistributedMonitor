#define TOKEN_H
#include "../message/message.h"
#include <vector>
#include <queue>

using namespace std;
class Token{
    public:
    vector<int> getLN();
    queue<int> getRequestProcess();
    void addRequestProcess(int port);
    void setLN(int position,int sn);
    void setRequestProcess(queue<int> LN);
    void setToken(Message message);
    int getFirstProcessFromQueue();
    void addNewProcesToLN();


    private:
    vector<int> LN;
    queue<int> requestProcess;


};