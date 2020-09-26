#define MESSAGE_H
#include <chrono>
#include <vector>
#include <queue>
#include<iostream>
#include<string>


using namespace std;
class Message {
    public:
        Message(string messageSerialized);
        Message(string messageType,int port,int sn);
        Message(string messageType,int port,vector<int> LN,queue<int> requestProcess);
        string messageSerialize();
        Message messageDeserialize();
        vector<int> getLN();
        queue<int> getRequestProcess();
        int getPort();
        int getSn();
        string getMessageType();
    private:
        int port,sn;
        string messageSerialized;
        vector<int> LN; 
        queue<int> requestProcess;
        string messageType; 
        string vectorToString(vector<int> LN);
        string queueToString(queue<int> requestProcess);
        vector<string> fragmentString(const string &txt, char ch);

};