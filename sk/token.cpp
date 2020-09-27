#include "token.h"


vector<int> Token::getLN(){return LN;};

queue<int> Token::getRequestProcess(){return requestProcess;};

void Token::addRequestProcess(int port){
    requestProcess.push(port);
}

void Token::setLN(int position,int value){
    LN[position] = value;
}

void Token::setRequestProcess(queue<int> requestProcess){
    this->requestProcess = requestProcess;
}


void Token::setToken(Message message){
    LN = message.getLN();
    requestProcess = message.getRequestProcess();
}

int Token::getFirstProcessFromQueue(){
    int port = requestProcess.front();
    requestProcess.pop();
    return port;
}

 void Token::addNewProcesToLN(){
     LN.push_back(0);
 }