
//#include "./sk/sk.h"
#include "./monitor/monitor.h"
#include <algorithm>

using namespace std;

void test_messageSerializeT(){ 
    vector<int> LN = {1,3,7};
    queue<int> requestProcess;
    //requestProcess.push(1);
    //requestProcess.push(2);
    //requestProcess.push(3);
    int port = 1250;
    Message messsage{"T",port,LN,requestProcess};
    if(messsage.messageSerialize()=="T,1250,1;3;7,empty"){//nadal bład ale nie moge się skupić
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
    Message messageSeralized{"T,1250,1;3;7,empty"};
    Message messageDeserialized = messageSeralized.messageDeserialize();
    vector<int> LN = {1,3,7};
    queue<int> requestProcess;
   // requestProcess.push(1);
    //requestProcess.push(2);
    //requestProcess.push(3);
    int port = 1250;
    if(messageDeserialized.getMessageType()=="T" && LN==messageDeserialized.getLN() && requestProcess==messageDeserialized.getRequestProcess() && messageDeserialized.getPort()==port){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};

void test_reciveMessage(){
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


void test_tokenMessage(){
    SK sk{1250,false};
    sk.addProcessToRN(1251);
    sk.addProcessToRN(1252);
    sk.addProcessToRN(1253);
    sk.addProcessToRN(1254);
    queue<int> simpleQueue;
    simpleQueue.push(1251);
    simpleQueue.push(1252);
    simpleQueue.push(1253);
    vector<int> LN = {0,1,0,0,0};
    Message message{"T",1250,LN,simpleQueue};
    Message messageSerialized = message.messageSerialize();
    sk.reciveMessage(messageSerialized);
    Token token = sk.getToken();
    if(sk.getUseToken() == true && token.getLN()==LN && token.getRequestProcess()==simpleQueue){
        cout<<"OK"<<endl;
    }
    else{
        cout<<"FAIL"<<endl;
    }
}

void test_requestMessage(){
    SK sk{1250,false};
    sk.addProcessToRN(1251);
    sk.addProcessToRN(1252);
    sk.addProcessToRN(1253);
    sk.addProcessToRN(1254);
    sk.requestMessage();
}

void test_producer(queue<int> *buffer, size_t size){
    vector<int> other;
    other.push_back(1251);
    other.push_back(1252);
    Monitor monit{1250,true,other};
    for(int i=0; i<100; i++){
        monit.in();
        if(buffer->size()<size){
        buffer->push(i);
        }else{
            cout<<"Buffer full"<<endl;
        }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}

void test_consument1(queue<int> *buffer, size_t size){
    vector<int> other;
    other.push_back(1250);
    other.push_back(1252);
    Monitor monit{1251,false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"cons1: "<<endl;
       // if(buffer->size()!=0){
            //cout<<"buffer element: "<<&buffer->front()<<endl;
      //      buffer->pop();
       // }else{
            cout<<"Buffer empty"<<endl;//naprawić buffor
       // }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}

void test_consument2(queue<int> *buffer, size_t size){
    vector<int> other;
    other.push_back(1251);
    other.push_back(1250);
    Monitor monit{1252,false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"cons1: "<<endl;
        //if(buffer->size()!=0){
            //cout<<"buffer element: "<<&buffer->front()<<endl;
           // buffer->pop();
        //}else{
            cout<<"Buffer empty"<<endl;
       // }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}

int main(int argc,char **argv){
    test_messageSerializeT();
    test_messageSerializeR();
    test_messageDeserializeT();
    //test_reciveMessage();
    //test_tokenMessage();
    //test_requestMessage();
    queue<int> *buffer;
    size_t size = 5;
    switch (stoi(argv[1]))
    {
    case 1:
        test_producer(buffer,size);
        break;
    case 2:
        test_consument1(buffer,size);
        break;
    case 3:
        test_consument2(buffer,size);
        break;
    default:
        break;
    }
    return 0;
};
