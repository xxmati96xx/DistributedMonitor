
#include "./sk/sk.h"

using namespace std;

void test_messageSerializeT(){ 
    vector<int> LN = {1,3,7};
    queue<int> requestProcess;
    requestProcess.push(1);
    requestProcess.push(2);
    requestProcess.push(3);
    int port = 1250;
    Message messsage{"T",port,LN,requestProcess};
    if(messsage.messageSerialize()=="T,1250,1;3;7,1;2;3"){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};
void test_messageSerializeR(){ 
    int port = 1250;
    int sn = 2;
    Message messsage{"R",port,sn};
    if(messsage.messageSerialize()=="R,1250,2"){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};

void test_messageDeserializeT(){ 
    Message messageSeralized{"T,1250,1;3;7,1;2;3"};
    Message messageDeserialized = messageSeralized.messageDeserialize();
    vector<int> LN = {1,3,7};
    queue<int> requestProcess;
    requestProcess.push(1);
    requestProcess.push(2);
    requestProcess.push(3);
    int port = 1250;
    if(messageDeserialized.getMessageType()=="T" && LN==messageDeserialized.getLN() && requestProcess==messageDeserialized.getRequestProcess() && messageDeserialized.getPort()==port){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};

void test_requestMessage(){
    SK sk{1250,true};
    sk.addProcessToRN(1251);
    sk.addProcessToRN(1252);
    sk.addProcessToRN(1253);
    sk.addProcessToRN(1254);
    vector<int> LN = {0,0,0,0,0};
    Token token = sk.getToken();
    token.setLN(LN);
    sk.setToken(token);
    Message message{"R,1251,1"};
    sk.reciveMessage(message);
    message = {"R,1251,2"};
    sk.reciveMessage(message);
    message = {"R,1252,1"};
    sk.reciveMessage(message);
    message = {"R,1253,2"};
    sk.reciveMessage(message);
    message = {"R,1254,5"};
    sk.reciveMessage(message);
    bool t1,t2,t3,t4;
    vector<pair<int,int>> RN = sk.getRN();
    for(size_t i=0;i<RN.size();i++){
        if(RN[i].first == 1251 && RN[i].second == 2){t1=true;};
        if(RN[i].first == 1252 && RN[i].second == 1){t2=true;};
        if(RN[i].first == 1253 && RN[i].second == 2){t3=true;};
        if(RN[i].first == 1254 && RN[i].second == 5){t4=true;};
    }
    if(t1 && t2 && t3 && t4){
        cout<<"OK"<<endl;
    }else{
        cout<<"FAIL"<<endl;
    }
}

int main(){
    test_messageSerializeT();
    test_messageSerializeR();
    test_messageDeserializeT();
    test_requestMessage();
};
