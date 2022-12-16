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
        int fd = sc.getFD();

        int lu;
        read(fd, &lu, sizeof(int));
        std::cout << "lu =" << lu << std::endl;
        lu++;
        write(fd, &lu, sizeof(int));
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
