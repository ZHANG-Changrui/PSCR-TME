#include "TCPServer.h"
#include <vector>
#include <thread>
#include "ServerSocket.cpp"
#include "Socket.cpp"
#include "ConnectionHandler.cpp"
using namespace std;
using namespace pr;

int main(){
    TCPServer tcp;
    tcp.startServer(1664);
}
void pr::TCPServer::startServer(int port) {
        ss=new ServerSocket(port);
        while(1) {
            Socket sc = ss->accept();//bloquant
            threads.emplace_back(handlerClient, sc);
        }
}