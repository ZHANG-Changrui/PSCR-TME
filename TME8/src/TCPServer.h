#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_
#include <unistd.h>
#include <thread>
#include "ServerSocket.h"
#include "ConnectionHandler.h"
#include <thread>
#include <vector>
namespace pr {

// un serveur TCP, la gestion des connections est déléguée
    void handlerClient(Socket sc){//discution client
        int N;
        read(sc.getFD(),&N,sizeof(N));
        ++N;
        write(sc.getFD(),&N,sizeof(N));
        sc.close();
    }
    class TCPServer{
        ServerSocket *ss;
        std::vector<std::thread> threads;
        //ConnectionHandler *handler;
        //std::thread* waitThread;
    public:
        void startServer(int port);
        void stopServer();
        ~TCPServer(){
            delete ss;
            for(auto &i:threads){
                i.join();
            }
        }
    };
}

#endif /* SRC_TCPSERVER_H_ */
