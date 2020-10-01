#define TOKEN_H
#include "../message/message.h"
#include <vector>
#include <queue>

using namespace std;
class Token{
    public:
    vector<int> getLN();
    queue<string> getRequestProcess();
    void addRequestProcess(string address);
    void setLNPosition(int position,int sn);
    void setRequestProcess(queue<string> requestProcess);
    void setToken(Message message);
    string getFirstProcessFromQueue();
    void addNewProcesToLN();
    void setLN(vector<int> LN);


    private:
    vector<int> LN;
    queue<string> requestProcess;


};