
//#include "./sk/sk.h"
#include "./monitor/monitor.h"
#include <algorithm>

using namespace std;

void test_messageSerializeT(){ 
    vector<int> LN = {1,3,7};
    queue<string> requestProcess;
    //requestProcess.push(1);
    //requestProcess.push(2);
    //requestProcess.push(3);
    string address = "192.168.6.10:1520";
    Message messsage{"T",address,LN,requestProcess};
    if(messsage.messageSerialize()=="T,192.168.6.10:1520,1;3;7,empty"){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};
void test_messageSerializeR(){ 
    string address = "192.168.6.10:1520";
    int sn = 2;
    Message messsage{"R",address,sn};
    if(messsage.messageSerialize()=="R,192.168.6.10:1520,2"){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};

void test_messageDeserializeT(){ 
    Message messageSeralized{"T,192.168.6.10:1520,1;3;7,empty"};
    Message messageDeserialized = messageSeralized.messageDeserialize();
    vector<int> LN = {1,3,7};
    queue<string> requestProcess;
   // requestProcess.push(1);
    //requestProcess.push(2);
    //requestProcess.push(3);
     string address = "192.168.6.10:1520";
    if(messageDeserialized.getMessageType()=="T" && LN==messageDeserialized.getLN() && requestProcess==messageDeserialized.getRequestProcess() && messageDeserialized.getAddress()==address){
        cout<<"OK"<<endl;
    }
    else
    {
        cout<<"FAIL"<<endl;
    }
};

void test_reciveMessage(){
    vector<string> other;
    SK sk{"192.168.6.10:1250",true};
    other.push_back("192.168.6.10:1251");
    other.push_back("192.168.6.10:1252");
    other.push_back("192.168.6.10:1253");
    other.push_back("192.168.6.10:1254");
    sk.addProcessToRN(other);
    vector<int> LN = {0,0,0,0,0};
    Token token = sk.getToken();
    token.setLN(LN);
    sk.setToken(token);
    Message message{"R,192.168.6.10:1251,1"};
    sk.reciveMessage(message);
    message = {"R,192.168.6.10:1251,2"};
    sk.reciveMessage(message);
    message = {"R,192.168.6.10:1252,1"};
    sk.reciveMessage(message);
    message = {"R,192.168.6.10:1253,2"};
    sk.reciveMessage(message);
    message = {"R,192.168.6.10:1254,5"};
    sk.reciveMessage(message);
    bool t1,t2,t3,t4;
    vector<pair<string,int>> RN = sk.getRN();
    for(size_t i=0;i<RN.size();i++){
        if(RN[i].first == "192.168.6.10:1251" && RN[i].second == 2){t1=true;};
        if(RN[i].first == "192.168.6.10:1252" && RN[i].second == 1){t2=true;};
        if(RN[i].first == "192.168.6.10:1253" && RN[i].second == 2){t3=true;};
        if(RN[i].first == "192.168.6.10:1254" && RN[i].second == 5){t4=true;};
    }
    if(t1 && t2 && t3 && t4){
        cout<<"OK"<<endl;
    }else{
        cout<<"FAIL"<<endl;
    }
}


void test_tokenMessage(){
    vector<string> other;
    SK sk{"192.168.6.10:1250",false};
    other.push_back("192.168.6.10:1251");
    other.push_back("192.168.6.10:1252");
    other.push_back("192.168.6.10:1253");
    other.push_back("192.168.6.10:1254");
    sk.addProcessToRN(other);
    queue<string> simpleQueue;
    simpleQueue.push("192.168.6.10:1251");
    simpleQueue.push("192.168.6.10:1252");
    simpleQueue.push("192.168.6.10:1253");
    vector<int> LN = {0,1,0,0,0};
    Message message{"T","192.168.6.10:1250",LN,simpleQueue};
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
    vector<string> other;
    SK sk{"192.168.6.10:1250",false};
    other.push_back("192.168.6.10:1251");
    other.push_back("192.168.6.10:1252");
    other.push_back("192.168.6.10:1253");
    other.push_back("192.168.6.10:1254");
    sk.addProcessToRN(other);
    sk.requestMessage();
}

string dataToString(queue<int> queue){
    string queueString ="";
    while (queue.size()>0)
    {
        queueString += to_string(queue.front())+"|";
        queue.pop();
    }
    if(queueString.size()>0){
        queueString.pop_back();
    }
    cout<<"queue: "<<queueString<<endl;
    return queueString;
}

vector<string> fragmentString(const string &txt, char ch)
{
    vector<string> strs;
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs;
}

queue<int> stringToData(string data){
    queue<int> queue;
    if(data.size()>0){
        vector<string> fragmentStringData = fragmentString(data,'|');
        if(fragmentStringData.size()>0){
            for(size_t i=0; i<fragmentStringData.size();i++){
                queue.push(stoi(fragmentStringData[i]));
            }
        }
    }
    return queue;
}

void test_producer1(string address,string token, string otherAddress){
    size_t size = 5;
    vector<string> other;
    other.push_back(address);
    vector<string> otherAddresses = fragmentString(otherAddress,',');
    for(size_t i=0;i<otherAddresses.size();i++){
        other.push_back(otherAddresses[i]);
    }
    Monitor monit{address,(bool)stoi(token),other};
    int i=0;
    while (i<100){
        monit.in();
         queue<int> queue = stringToData(monit.getData());
        if(queue.size()<size){
            cout<<"Push element: "<<i<<endl;
            queue.push(i);
            i++;
        }else{
            cout<<"Queue full"<<endl;
        }
        monit.setData(dataToString(queue));
        monit.out();
         this_thread::sleep_for (std::chrono::milliseconds(1000));
         
    }
    cout<<"end"<<endl;
}

void test_consument1(string address,string token, string otherAddress){
    vector<string> other;
    other.push_back(address);
    vector<string> otherAddresses = fragmentString(otherAddress,',');
    for(size_t i=0;i<otherAddresses.size();i++){
        other.push_back(otherAddresses[i]);
    }
    Monitor monit{address,(bool)stoi(token),other};
    int i=0;
    while (i<100)
    {
        monit.in();
        
            queue<int> queue = stringToData(monit.getData());
        
        if(queue.size()!=0){
            int item = queue.front();
            queue.pop();
            cout<<"Take element: "<<item<<endl;
            i++;
        }else{
            cout<<"Queue empty"<<endl;
        }
        monit.setData(dataToString(queue));
        monit.out();
         this_thread::sleep_for (std::chrono::milliseconds(1000));
         
    }
    cout<<"end"<<endl;
}

void test_dataSendInTokenMessage(){
    vector<string> other;
    other.push_back("127.0.0.1:1250");
    Monitor monit{"127.0.0.1:1251",true,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"To dostałem do innego procesu: "<<monit.getData()<<endl;
        string str = "To jest test wiadomosci";
        cout<<"Wyswietl wiadomosc "<<str<<endl;
        monit.setData(str);
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(1));
    }
}
void test_dataSendInTokenMessage1(){
    vector<string> other;
    other.push_back("127.0.0.1:1251");
    Monitor monit{"127.0.0.1:1250",false,other};
    for(int i=0; i<100; i++){
        monit.in();
        cout<<"To dostałem do innego procesu: "<<monit.getData()<<endl;
        string str = "To jest inna wiadomosc do testu";
        cout<<"Wyswietl wiadomosc "<<str<<endl;
        monit.setData(str);
        monit.out();
         this_thread::sleep_for (std::chrono::seconds(1));
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
        test_producer1(argv[2],argv[3],argv[4]);
        break;
    case 2:
        test_consument1(argv[2],argv[3],argv[4]);
        break;
    default:
        break;
    }
    return 0;
};
