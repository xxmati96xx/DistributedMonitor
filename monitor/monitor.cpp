#include "monitor.h"

Monitor::Monitor(string address,bool useToken,vector<string> other):sk(address,useToken){
    other.push_back(address);
    sort(other.begin(),other.end());
    for(size_t i=0; i<other.size();i++){
        sk.addProcessToRN(other[i]);
        if(useToken){
            sk.initLN();
        }
    }
    context = zmq_ctx_new();
    socket = zmq_socket(context,ZMQ_REP);
    string bind = "tcp://"+address;
    zmq_bind(socket,bind.c_str());

    thread handlerThread(&Monitor::receiveMessage, this);
    handlerThread.detach();


}

Monitor::~Monitor(){
    zmq_close(socket);
    zmq_ctx_destroy(context);
}

void Monitor::in(){
    if(sk.getUseToken()){

    }else{
        sk.requestMessage();
        while (!sk.getUseToken())
        {
            this_thread::sleep_for (std::chrono::milliseconds(10));
        }
    }
}

void Monitor::out(){
    sk.endCS();
}


void Monitor::receiveMessage(){
    char *receiveBUff = new char[BUFSIZ];
    while(1){
        memset(receiveBUff,0,BUFSIZ);
        if(zmq_recv(socket,receiveBUff,BUFSIZ,0)>-1){
            zmq_send(socket,"",0,0);
            string message(receiveBUff);
            //cout<<message<<endl;
            sk.reciveMessage(message);
        }
    }
}

void Monitor::setData(string data){
    sk.setData(data);
}

string Monitor::getData(){
    return sk.getData();
}