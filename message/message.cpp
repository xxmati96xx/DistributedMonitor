#include "message.h"



Message::Message(string messageSerialized){
    this->messageSerialized = messageSerialized;
}
Message::Message(string messageTypeA,int portA,int snA):port(portA),sn(snA){
    messageType = messageTypeA;
}
Message::Message(string messageTypeA,int portA,vector<int> LNA,queue<int> requestProcessA):port(portA),LN(LNA),requestProcess(requestProcessA){
    messageType = messageTypeA;
}

Message Message::messageDeserialize(){
    vector<string> fragmentStringMessage = fragmentString(messageSerialized,',');
    if(fragmentStringMessage[0]=="R"){
        return Message{"R",stoi(fragmentStringMessage[1]),stoi(fragmentStringMessage[2])};
    }else if(fragmentStringMessage[0]=="T"){
        vector<string> LNString = fragmentString(fragmentStringMessage[2],';');
        vector<int> LN;
        for(size_t i=0;i<LNString.size();i++){
            LN.push_back(stoi(LNString[i]));
        }
        vector<string> requestProcessString = fragmentString(fragmentStringMessage[3],';');
        if(requestProcessString[0] != "empty"){
            for(size_t i=0;i<requestProcessString.size();i++){
                requestProcess.push(stoi(requestProcessString[i]));
            }
        }
        Message mes{"T",stoi(fragmentStringMessage[1]),LN,requestProcess};
        if(fragmentStringMessage.size()>4){
        mes.setData(fragmentStringMessage[4]);
        }
        return mes;
        //return Message{"T",stoi(fragmentStringMessage[1]),LN,requestProcess};
    }
    return Message{"B",0,0};
}

string Message::messageSerialize(){
    string messageString = "";
    if(getMessageType() == "R"){
        messageString += getMessageType()+",";
        messageString += to_string(getPort())+",";
        messageString += to_string(getSn());
    }else if(getMessageType() == "T"){
        messageString += getMessageType()+",";
        messageString += to_string(getPort())+",";
        messageString += vectorToString(getLN())+",";
        messageString += queueToString(getRequestProcess());
        if(data.size()>0){
        messageString += ","+data;
        }
    }
    return messageString;
}



string Message::getMessageType(){return messageType;};

int Message::getPort() {return port;};

int Message::getSn() {return sn;};

vector<int> Message::getLN() {return LN;};
queue<int> Message::getRequestProcess(){return requestProcess;};

string Message::vectorToString(vector<int> LN){
    string vectorSting ="";
    for(size_t i=0; i<LN.size()-1;i++){
        vectorSting += to_string(LN.at(i))+";";
    }
    vectorSting += to_string(LN.at(LN.size()-1)); //last element without ";"
    return vectorSting;
}

string Message::queueToString(queue<int> requestProcess){
    queue<int> request = requestProcess;
    int queueSize = request.size();
    string queueString = "";
    if(queueSize >0){
    for(int i=0; i<queueSize; i++){
         queueString += to_string(request.front())+";";
        request.pop();
    }
    queueString.pop_back(); //remove last element ";"
    return queueString;
    }
    queueString += "empty";
    return queueString;
}
//Kod z internetu {
vector<string> Message::fragmentString(const string &txt, char ch)
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
//Koniec kod z internetu }

void Message::setData(string data){
    this->data = data;
}

string Message::getData(){
    return data;
}