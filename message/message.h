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
        Message(string messageType,string address,int sn);
        Message(string messageType,string address,vector<int> LN,queue<string> requestProcess);
        string messageSerialize();
        Message messageDeserialize();
        vector<int> getLN();
        queue<string> getRequestProcess();
        void setData(string data);
        string getData();
        string getAddress();
        int getSn();
        string getMessageType();
    private:
        int sn;
        string address;
        string messageSerialized;
        vector<int> LN; 
        queue<string> requestProcess;
        string messageType; 
        string data;
        string vectorToString(vector<int> LN);
        string queueToString(queue<string> requestProcess);
        vector<string> fragmentString(const string &txt, char ch);

};