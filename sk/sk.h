#define SK_H
#include "token.h"
#include <vector>
#include <queue>
#include <zmq.h>

using namespace std;
class SK{
    public:
    void addProcessToRN(string address);
    void requestMessage();
    void tokenMessage();
    void reciveMessage(Message message);
    void endCS();
    void initLN();
    Token getToken();
    bool getUseToken();
    void setData(string data);
    string getData();
    void setToken(Token token);
    vector<pair<string,int>> getRN();
    SK(string address, bool useToken);
    ~SK();

    private:
    Message getRequestMessage();
    bool isInRequestProcess(string address);
    string address;
    Token token;
    bool useToken;
    string data;
    void sendMessage(Message message,string address);
    void *context;
    vector<pair<string,int>> RN;

};