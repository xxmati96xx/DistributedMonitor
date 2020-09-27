#include "sk.h"


void SK::addProcessToRN(int port){
    RN.push_back(make_pair(port,0));
}

void SK::requestMessage(){
    for(pair<int,int> item:RN){
        if(item.first == port){
            item.second++; 
            break;
        }
    }
    Message message = getRequestMessage();
    for(pair<int,int> item:RN){
        if(item.first != port){
            sendMessage(message,item.first);
        }
    }
}
    
void SK::tokenMessage(){
    useToken = false;
    int destination = token.getFirstProcessFromQueue();
    Message message{"T",destination,token.getLN(),token.getRequestProcess()};
    sendMessage(message,destination);
}


void SK::sendMessage(Message message,int port){
    string messageSerialized = message.messageSerialize();
    void *socket = zmq_socket(context,ZMQ_REQ);
    string destination = "tcp://127.0.0.1:"+to_string(port);
    if(zmq_connect(socket,destination.c_str())==0){
        zmq_send(socket,messageSerialized.c_str(),sizeof(messageSerialized),0);
    }
    else{
        cout<<"zmq_error"<<zmq_strerror(zmq_errno())<<endl;
    }
    zmq_close(socket);
}

void SK::reciveMessage(Message message){
    Message message = message.messageDeserialize();
    if(message.getMessageType() == "R"){
        for(pair<int,int> item:RN){
            if(item.first == message.getPort()){
                item.second = max(item.second,message.getSn());
                break;
            }
        }
        if(useToken && token.getRequestProcess().size()==0){
            for(int i=0; i<RN.size(); i++){
                if(RN[i].second == token.getLN()[i]+1){
                    token.addRequestProcess(RN[i].first);
                }
            }
            if(token.getRequestProcess().size()>0)
            tokenMessage();
        }
    }
    else if(message.getMessageType() == "T"){
        token.setToken(message);
        useToken = true;
    }
}


Message SK::getRequestMessage(){
    for(pair<int,int> item:RN){
        if(item.first == port){
            return {"R",port,item.second};
        }
    
    }
    return {"F",0,0};
}

void SK::endCS(){
    for(int i=0; i<RN.size(); i++){
        if(RN[i].first == port){
            token.setLN(i,RN[i].second);
            break;
        }
    }
}

bool SK::isInRequestProcess(int port){
    
}

SK::SK(int port,bool useToken){
    this->useToken = useToken;
    this->port = port;
    context = zmq_ctx_new();
}

SK::~SK(){
    zmq_ctx_destroy(context);
}


