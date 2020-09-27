#define SK_H
#include "token.h"
#include <vector>
#include <queue>
#include <zmq.h>

using namespace std;
class SK{
    public:
    void addProcessToRN(int port);
    void requestMessage();
    void tokenMessage();
    void reciveMessage(Message message);
    void endCS();
    bool isCS();
    Token getToken();
    void setToken(Token token);
    vector<pair<int,int>> getRN();
    SK(int port, bool useToken);
    ~SK();

    private:
    Message getRequestMessage();
    bool isInRequestProcess(int port);
    int port;
    Token token;
    bool useToken;
    void sendMessage(Message message,int port);
    void *context;
    vector<pair<int,int>> RN;

};