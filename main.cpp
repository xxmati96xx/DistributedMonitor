
//#include "./sk/sk.h"
#include "./monitor/monitor.h"
#include <algorithm>

using namespace std;

//queue<int> *buffer;
//size_t size;

void test_messageSerializeT(){ 
    vector<int> LN = {1,3,7};
    queue<int> requestProcess;
    //requestProcess.push(1);
    //requestProcess.push(2);
    //requestProcess.push(3);
    int port = 1250;
    Message messsage{"T",port,LN,requestProcess};
    if(messsage.messageSerialize()=="T,1250,1;3;7,empty"){
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
/*
void test_producer(){
    //queue<int> queue;
    //cout<<"adres queue: "<<&queue<<endl;
    buffer = new queue<int>;
    cout<<"adres buffer: "<<&buffer<<endl;
    size = 5;
    vector<int> other;
    other.push_back(1251);
    other.push_back(1252);
    Monitor monit{1250,true,other};
    for(int i=0; i<100; i++){
        monit.in();
        if(buffer->size()<size){
            cout<<i<<endl;
        buffer->push(i);
        cout<<"buffer element:push "<<buffer->front()<<endl;
        }else{
            cout<<"Buffer full"<<endl;
            //for(size_t i=0;i<size;i++){
            //    cout<<"buffer element: "<<buffer.front()<<endl;
                buffer->pop();
            //}
        }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(1));
    }
}

void test_consument1(){
    vector<int> other;
    other.push_back(1250);
    other.push_back(1252);
    Monitor monit{1251,false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"cons1: "<<endl;
        if(buffer.size()!=0){
            cout<<"buffer element: "<<buffer.front()<<endl;
            buffer.pop();
        }else{
            cout<<"Buffer empty"<<endl;//naprawić buffor
        }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}

void test_consument2(){
    vector<int> other;
    other.push_back(1251);
    other.push_back(1250);
    Monitor monit{1252,false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"cons1: "<<endl;
        if(buffer.size()!=0){
            cout<<"buffer element: "<<buffer.front()<<endl;
            buffer.pop();
        }else{
            cout<<"Buffer empty"<<endl;
        }
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}

*/




void test_dataSendInTokenMessage(){
    vector<int> other;
    other.push_back(1250);
    Monitor monit{1251,true,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"To dostałem do innego procesu: "<<monit.getData()<<endl;
        string str = "To jest test wiadomosci";
        cout<<"Wyswietl wiadomosc "<<str<<endl;
        monit.setData(str);
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(3));
    }
}
void test_dataSendInTokenMessage1(){
    vector<int> other;
    other.push_back(1251);
    Monitor monit{1250,false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"To dostałem do innego procesu: "<<monit.getData()<<endl;
        string str = "To jest inna wiadomosc do testu";
        cout<<"Wyswietl wiadomosc "<<str<<endl;
        monit.setData(str);
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
    
    switch (stoi(argv[1]))
    {
    case 1:
        test_dataSendInTokenMessage();
        break;
    case 2:
        //test_consument1();
        test_dataSendInTokenMessage1();
        break;
    case 3:
        //test_consument2();
        break;
    default:
        break;
    }
  

    vector<string> addres;
    addres.push_back("192.168.0.15:1251");
    addres.push_back("192.168.0.16:1251");
    addres.push_back("192.168.0.20:1251");
    addres.push_back("027.0.0.1:1242");
    addres.push_back("192.168.0.1:1251");
    addres.push_back("127.0.0.1:1242");
    sort(addres.begin(),addres.end());
    for(size_t i=0;i<addres.size();i++){
        cout<<"vector element1: "<<addres[i]<<endl;
    }
    vector<string> addres1;
    addres1.push_back("192.168.0.16:1251");
    addres1.push_back("192.168.0.15:1251");
    addres1.push_back("192.168.0.20:1251");
    addres1.push_back("027.0.0.1:1242");
    addres1.push_back("192.168.0.1:1251");
    addres1.push_back("127.0.0.1:1242");
    sort(addres1.begin(),addres1.end());
    for(size_t i=0;i<addres1.size();i++){
        cout<<"vector element2: "<<addres1[i]<<endl;
    }
    vector<string> addres2;
    addres2.push_back("027.0.0.1:1242");
    addres2.push_back("192.168.0.16:1251");
    addres2.push_back("192.168.0.15:1251");
    addres2.push_back("192.168.0.20:1251");
    addres2.push_back("192.168.0.1:1251");
    addres2.push_back("127.0.0.1:1242");
    sort(addres2.begin(),addres2.end());
    for(size_t i=0;i<addres2.size();i++){
        cout<<"vector element3: "<<addres2[i]<<endl;
    }
    vector<string> addres3;
    string st = "192.168.0.20:1251";
    addres3.push_back("192.168.0.1:1251");
    addres3.push_back("192.168.0.16:1251");
    addres3.push_back("192.168.0.15:1251");
    addres3.push_back("192.168.0.20:1251");
    addres3.push_back("027.0.0.1:1242");
    addres3.push_back("127.0.0.1:1242");
    sort(addres3.begin(),addres3.end());
    for(size_t i=0;i<addres3.size();i++){
        if(st == addres3[i]){
            cout<<"Działa"<<endl;
        }
        cout<<"vector element4: "<<addres3[i]<<endl;
    }
    if((addres == addres1) && (addres2 == addres3) && (addres == addres2) && (addres1 == addres3) && (addres == addres3) && (addres1 == addres2)){
        cout<<"OK"<<endl;
    }else
    {
       cout<<"FAIL"<<endl;
    }
    vector<pair<string,int>> RN;
    RN.push_back(make_pair("192.168.0.20:1251",0));
    RN.push_back(make_pair("192.168.0.5:1251",0));
    RN.push_back(make_pair("192.168.0.18:1251",0));
    RN.push_back(make_pair("192.168.0.20:1245",0));
     for (auto it = begin (RN); it != end (RN); ++it) {
        if(it->first==st) it->second++;     
    }
    for (auto it = begin (RN); it != end (RN); ++it) {
        cout<<it->first<<" addres "<<it->second<<" SN value"<<endl;     
    }
    
   
    return 0;
};
