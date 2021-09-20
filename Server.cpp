
#include "Server.h"

Server::Server(int port)throw (const char*) {
    this->fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd<0)
        throw "socket failed";
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    this->isRunning = true;
    if (bind(fd,(struct sockaddr*)&server,sizeof(server))<0)
        throw "bind failure";
    if(listen(fd,3)<0)//gets number of clients////////////////////////////////////
        throw "listen failure";

}

void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch,this]() {
        socklen_t clientSize = sizeof(client);
        while (isRunning) {

            alarm(5);
//                fcntl(sock_desc, F_SETFL, fcntl(sock_desc, F_GETFL, 0) | O_NONBLOCK);`
            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            if (aClient < 0)
                throw "client failure";
            ch.handle(aClient);
            close(aClient);
            sleep(2);
        }
            close(fd);
//        }

    });
}



void Server::stop(){
    this->isRunning = false;
    t->join(); // do not delete this!

}

Server::~Server() {
//    delete t;
}

