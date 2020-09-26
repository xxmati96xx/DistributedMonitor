#include "./message/message.h"

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
        cout<<"NO OK"<<endl;
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
        cout<<"Fail"<<endl;
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
        cout<<"NO OK"<<endl;
    }
};

int main(){
    test_messageSerializeT();
    test_messageSerializeR();
    test_messageDeserializeT();
};
