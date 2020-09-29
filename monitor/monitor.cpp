#include "monitor.h"

Monitor::Monitor(int port,bool useToken,vector<int> other):sk(port,useToken){
    other.push_back(port);
    sort(other.begin(),other.end());
    for(size_t i=0; i<other.size();i++){
        sk.addProcessToRN(other[i]);
        if(useToken){
            sk.initLN();
        }
    }
    context = zmq_ctx_new();
    socket = zmq_socket(context,ZMQ_REP);
    string bind = "tcp://127.0.0.1:"+to_string(port);
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
            this_thread::sleep_for (std::chrono::seconds(1));
        }
    }
}

void Monitor::out(){
    sk.endCS();
}


void Monitor::receiveMessage(){
    char *receiveBUff = new char[128];
    while(1){
        memset(receiveBUff,0,128);
        if(zmq_recv(socket,receiveBUff,128,0)>-1){
            zmq_send(socket,"",0,0);
            string message(receiveBUff);
            cout<<message<<endl;
            sk.reciveMessage(message);
        }
    }
}