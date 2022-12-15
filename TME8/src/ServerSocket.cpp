#include "ServerSocket.h"
#include <iostream>
#include <unistd.h>


pr::ServerSocket::ServerSocket(int port) {
    socketfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr;
    adr.sin_family=AF_INET;
    adr.sin_port=htons(port);
    adr.sin_addr.s_addr=INADDR_ANY;
    bind(socketfd,(struct sockaddr*)&adr,sizeof(adr));
    //attention, port déja utilisé...
    listen(socketfd,0);
}
pr::Socket pr::ServerSocket::accept() {
    struct sockaddr_in exp;
    socklen_t sz=sizeof(exp);
    int fd=::accept(socketfd,(struct sockaddr*)&exp,&sz);
    if(fd==-1){perror("accept");}
    std::cout<<"connexion reçus de"<<&exp<<std::endl;//affiche sockaddr_in defini ostream&operator
    return Socket(fd);
}

void pr::ServerSocket::close() {
    if(isOpen()){
        shutdown(socketfd,SHUT_RDWR);
        ::close(socketfd);
    }
}

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * adr){
    char host[1024];
    //etape1 DNS
    //gethostbyaddr -> hostent
    ::getnameinfo((struct sockaddr*)adr,sizeof(struct sockaddr_in),host,1024,NULL,0,0);
    os<<"Machine"<<host<<std::endl;
    //IP listble: 127.0.0.1
    os<<inet_ntoa(adr->sin_addr)// inet_ntoa: inet: internet, n:net, to , a: ascii
      <<":"<< ntohs(adr->sin_port)<<std::endl; //ntohs: n=net, to ,h=host, s= short
    return os;
}