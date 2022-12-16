#include "Socket.h"
#include <unistd.h>
void pr::Socket::connect(const std::string &host, int port) {
    struct addrinfo* adr;
    //resolution d'addresse ->addresse
    getaddrinfo(host.c_str(),NULL,NULL,&adr);
    //(host,service,hints, )
    in_addr ipv4=((sockaddr_in*)adr->ai_addr)->sin_addr;
    //cast violent, list chainée IPV6?
    freeaddrinfo(adr);
    connect(ipv4,port);
}
void pr::Socket::connect(in_addr ipv4, int port) {
    fd=socket(AF_INET,SOCK_STREAM,0);
    // (A_Family,mode STREAM or DGRAM,protocol 0=auto stream=TCP DGRAM=UDP)
    struct sockaddr_in adr;
    adr.sin_family=AF_INET;//discriminant
    adr.sin_port=htons(port);
    adr.sin_addr=ipv4;
    ::connect(fd,(struct sockaddr*)&adr,sizeof(adr));
}

void pr::Socket::close() {
    if(isOpen()){
        shutdown(fd,SHUT_RDWR);
        ::close(fd);
    }
}
