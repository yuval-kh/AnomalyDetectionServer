/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include <netinet/in.h>
#include <string.h>
#include "CLI.h"

//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <sstream>


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class Socketio:public DefaultIO {
private:
    int clientID;
public:
    string read() {
        int buffer_size = 1024;
        char buffer[10] = {0};
        string output = "";
        ::read(this->clientID,buffer,1);
        while (buffer[0]!= '\n') {
        output += buffer[0];
        ::read(this->clientID,buffer,1);
        }
        return output;

    }
    void write(string text){
//        char buffer[text.size()+1] = text;
        ::send(clientID,text.c_str(),text.size(),0);
    }
    void write(float f){

        string out = std::to_string(f);
        std::ostringstream ss;
        ss << f;
        std::string s(ss.str());

        s+='\0';
        int i=0;
        while (s[i]!='\0') {
            char out = s[i];
            ::send(clientID, &out, 1, 0);
            i++;
        }

    }
    void read(float* f){
        char buffer = 0;
        string input = "";
        int n = ::read(this->clientID, &buffer, 1);
        while (buffer != '\n') {
            input += buffer;
            int n = ::read(this->clientID, &buffer, 1);
        }
        *f=stof(input);


    }
    Socketio(int clientID){
        this->clientID = clientID;
    }
    ~Socketio(){
//        <<"delete socketio\n";
    }
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        char buffer[1024];
//        int n = read(clientID,buffer,100);
//        const char* hello = "hello from server\n";
//        ::send(clientID,hello,strlen(hello),0);
//        int n = read(clientID,buffer,100);
//        cout<<n<<'\n';
//        cout<<buffer[0]<<buffer[1]<<'\n';

        Socketio* socketio = new Socketio(clientID);
        CLI* cli = new CLI(socketio);
        cli->start();
        delete socketio;
        delete cli;






    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    bool isRunning;
    int fd;
	sockaddr_in server;
    sockaddr_in client;


	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
