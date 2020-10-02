#include "monitor.h"

Monitor::Monitor(string address,bool useToken,vector<string> other):sk(address,useToken){
    other.push_back(address);
    sk.addProcessToRN(other);
    context = zmq_ctx_new();
    socket = zmq_socket(context,ZMQ_REP);
    int *noLinger = 0;
    zmq_setsockopt(socket,ZMQ_LINGER,noLinger,size_t(ZMQ_LINGER));
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
    sk.setUsedCS(true);
    if(sk.getUseToken()){

    }else{
        sk.requestMessage();
        int resend = 0;
        while (!sk.getUseToken())
        {
            this_thread::sleep_for (std::chrono::milliseconds(100));
            resend++;
            if(resend == 10){
                sk.reSendRequest();
                resend = 0;
            }

        }
    }
}

void Monitor::out(){
    sk.setUsedCS(false);
    sk.endCS();
}


void Monitor::receiveMessage(){
    char *receiveBUff = new char[BUFSIZ];
    while(1){
        memset(receiveBUff,0,BUFSIZ);
        if(zmq_recv(socket,receiveBUff,BUFSIZ,0)>-1){
            zmq_send(socket,"",0,0);
            string message(receiveBUff);
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