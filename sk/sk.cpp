#include "sk.h"


void SK::addProcessToRN(string address){
    RN.push_back(make_pair(address,0));
}

void SK::initLN(){
    token.addNewProcesToLN();
}

void SK::requestMessage(){
    for(size_t i=0; i<RN.size();i++){
        if(RN[i].first == address){
            RN[i].second = RN[i].second + 1; 
        }
    }
    Message message = getRequestMessage();
    for(size_t i=0; i<RN.size();i++){
        if(RN[i].first != address){
            sendMessage(message,RN[i].first);
        }
    }
}
    
void SK::reSendRequest(){
    Message message = getRequestMessage();
    for(size_t i=0; i<RN.size();i++){
        if(RN[i].first != address){
            sendMessage(message,RN[i].first);
        }
    }
}

void SK::tokenMessage(){
        string destination = token.getFirstProcessFromQueue();
        useToken = false;
        Message message{"T",destination,token.getLN(),token.getRequestProcess()};
        message.setData(data);
        sendMessage(message,destination);
}


void SK::sendMessage(Message message,string address){
    string messageSerialized = message.messageSerialize();
    void *socket = zmq_socket(context,ZMQ_REQ);
    string connect = "tcp://"+address;
    if(zmq_connect(socket,connect.c_str())==0){
        if(zmq_send(socket,messageSerialized.c_str(),messageSerialized.size(),0)>0){
           
        } 
        else{
            cout<<"zmq_error"<<zmq_strerror(zmq_errno())<<endl;
        }
    }
    else{
        cout<<"zmq_error"<<zmq_strerror(zmq_errno())<<endl;
    }
    zmq_close(socket);
}

void SK::reciveMessage(Message mes){
    Message message = mes.messageDeserialize();
    if(message.getMessageType() == "R"){
        if(message.getAddress() == "127.0.0.1:1251"){
            cout<<"resendmessagetest: "<<message.getAddress()<<" sn "<<message.getSn()<<endl;
        }
        for(size_t i=0; i<RN.size();i++){
            if(RN[i].first == message.getAddress()){
                RN[i].second = max(RN[i].second,message.getSn());
                break;
            }
        }
        if(useToken && token.getRequestProcess().size()==0 && !usedCS){
            vector<int> LN = token.getLN();  
            for(size_t i=0; i<RN.size(); i++){
                if(RN[i].second == LN[i]+1){
                    token.addRequestProcess(RN[i].first);
                }
            }
            if(token.getRequestProcess().size()>0){
            tokenMessage();
            }
        }
    }
    else if(message.getMessageType() == "T"){
        setData(message.getData());
        token.setToken(message);
        useToken = true;
    }
}


Message SK::getRequestMessage(){
    for(size_t i=0; i<RN.size(); i++){
        if(RN[i].first == address){
            return {"R",RN[i].first,RN[i].second};
        }
    
    }
    return {"F",0,0};
}

void SK::endCS(){
    for(size_t i=0; i<RN.size(); i++){
        if(RN[i].first == address){
            token.setLNPosition(i,RN[i].second);
        }else if(!isInRequestProcess(RN[i].first)){
            if(RN[i].second == token.getLN()[i]+1){
                token.addRequestProcess(RN[i].first);
            }
        }  
    }
    if(token.getRequestProcess().size()>0){
        tokenMessage();
    }
}

bool SK::isInRequestProcess(string address){
    queue<string> cp_requestProcess = token.getRequestProcess();
    string process;
    while(cp_requestProcess.size()>0){
        process = cp_requestProcess.front();
        cp_requestProcess.pop();
        if(process == address){return true;}
    }
    return false;
}

SK::SK(string address,bool useToken){
    this->useToken = useToken;
    this->address = address;
    context = zmq_ctx_new();
}

SK::~SK(){
    zmq_ctx_destroy(context);
}


vector<pair<string,int>> SK::getRN(){return RN;};

Token SK::getToken(){return token;};

void SK::setToken(Token token){
    this->token = token;
}

bool SK::getUseToken(){
    return useToken;
}

void SK::setData(string data){
    this->data = data;
}

string SK::getData(){
    return data;
}

void SK::setUsedCS(bool usedCS){
    this->usedCS = usedCS; 
}

bool SK::getUsedCS(){
    return usedCS;
}