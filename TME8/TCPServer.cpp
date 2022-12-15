#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include "ServerSocket.cpp"
#include "Socket.cpp"
#include "thread"
#include <vector>
using namespace std;
bool pr::TCPServer::startServer(int port) {
    ServerSocket ss(1200);
    vector<thread> threads;
    while(1) {
        Socket sc = ss.accept();
        //this->ss=ss.accept();
    }
}

void pr::TCPServer::stopServer() {

}