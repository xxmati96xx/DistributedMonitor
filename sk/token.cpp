#include "token.h"


vector<int> Token::getLN(){return LN;};

queue<string> Token::getRequestProcess(){return requestProcess;};

void Token::addRequestProcess(string address){
    requestProcess.push(address);
}

void Token::setLNPosition(int position,int value){
    LN[position] = value;
}

void Token::setRequestProcess(queue<string> requestProcess){
    this->requestProcess = requestProcess;
}


void Token::setToken(Message message){
    LN = message.getLN();
    requestProcess = message.getRequestProcess();
}

string Token::getFirstProcessFromQueue(){
    string address = requestProcess.front();
    requestProcess.pop();
    return address;
}

 void Token::addNewProcesToLN(){
     LN.push_back(0);
 }

 void Token::setLN(vector<int> LN){
     this->LN = LN;
 }