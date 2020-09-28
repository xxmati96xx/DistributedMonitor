#include "sk.h"


void SK::addProcessToRN(int port){
    RN.push_back(make_pair(port,0));
    token.addNewProcesToLN();
}

void SK::requestMessage(){
    for(pair<int,int> item:RN){
        if(item.first == port){
            cout<<"Test sn increment:przed "<<item.second<<endl;
            item.second++; 
            cout<<"Test sn increment:po "<<item.second<<endl;
            break;
        }
    }
    Message message = getRequestMessage();
    for(pair<int,int> item:RN){
        if(item.first != port){
            cout<<"Test port send: "<<item.first<<endl;
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
        //zmq_send(socket,messageSerialized.c_str(),sizeof(messageSerialized),0); dla testów wyłaczone 
    }
    else{
        cout<<"zmq_error"<<zmq_strerror(zmq_errno())<<endl;
    }
    zmq_close(socket);
}

void SK::reciveMessage(Message mes){
    Message message = mes.messageDeserialize();
    if(message.getMessageType() == "R"){
        for(auto it=begin(RN); it!=end(RN); ++it){
            if(it->first == message.getPort()){
                //cout<<"Add to RN second "<<it->first<<" "<<it->second<<endl;
                it->second = message.getSn();
                break;
            }
        }
        if(useToken && token.getRequestProcess().size()==0){
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
    for(size_t i=0; i<RN.size(); i++){
        if(RN[i].first == port){
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

bool SK::isInRequestProcess(int port){
    queue<int> cp_requestProcess = token.getRequestProcess();
    int process;
    while(cp_requestProcess.size()>0){
        process = cp_requestProcess.front();
        cp_requestProcess.pop();
        if(process == port){return true;}
    }
    return false;
}

SK::SK(int port,bool useToken){
    addProcessToRN(port);
    this->useToken = useToken;
    this->port = port;
    context = zmq_ctx_new();
}

SK::~SK(){
    zmq_ctx_destroy(context);
}


vector<pair<int,int>> SK::getRN(){return RN;};

Token SK::getToken(){return token;};

void SK::setToken(Token token){
    this->token = token;
}

bool SK::getUseToken(){
    return useToken;
}