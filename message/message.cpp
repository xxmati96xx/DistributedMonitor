#include "message.h"



Message::Message(string messageSerialized){
    this->messageSerialized = messageSerialized;
}
Message::Message(string messageTypeA,string address,int snA):sn(snA){
    this->address = address;
    messageType = messageTypeA;
}
Message::Message(string messageTypeA,string address,vector<int> LNA,queue<string> requestProcessA):LN(LNA){
    this->requestProcess = requestProcessA;
    messageType = messageTypeA;
    this->address = address;
}

Message Message::messageDeserialize(){
    vector<string> fragmentStringMessage = fragmentString(messageSerialized,',');
    if(fragmentStringMessage[0]=="R"){
        return Message{"R",fragmentStringMessage[1],stoi(fragmentStringMessage[2])};
    }else if(fragmentStringMessage[0]=="T"){
        vector<string> LNString = fragmentString(fragmentStringMessage[2],';');
        vector<int> LN;
        for(size_t i=0;i<LNString.size();i++){
            LN.push_back(stoi(LNString[i]));
        }
        vector<string> requestProcessString = fragmentString(fragmentStringMessage[3],';');
        queue<string> requestProcess;
        if(requestProcessString[0] != "empty"){
            for(size_t i=0;i<requestProcessString.size();i++){
                requestProcess.push(requestProcessString[i]);
            }
        }
        Message mes{"T",fragmentStringMessage[1],LN,requestProcess};
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
        messageString += getAddress()+",";
        messageString += to_string(getSn());
    }else if(getMessageType() == "T"){
        messageString += getMessageType()+",";
        messageString += getAddress()+",";
        messageString += vectorToString(getLN())+",";
        messageString += queueToString(getRequestProcess());
        if(data.size()>0){
        messageString += ","+data;
        }
    }
    return messageString;
}



string Message::getMessageType(){return messageType;};

string Message::getAddress() {return address;};

int Message::getSn() {return sn;};

vector<int> Message::getLN() {return LN;};
queue<string> Message::getRequestProcess(){return requestProcess;};

string Message::vectorToString(vector<int> LN){
    string vectorSting ="";
    for(size_t i=0; i<LN.size()-1;i++){
        vectorSting += to_string(LN.at(i))+";";
    }
    vectorSting += to_string(LN.at(LN.size()-1)); //last element without ";"
    return vectorSting;
}

string Message::queueToString(queue<string> requestProcess){
    queue<string> request = requestProcess;
    int queueSize = request.size();
    string queueString = "";
    if(queueSize >0){
    for(int i=0; i<queueSize; i++){
         queueString += request.front()+";";
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