#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include "chat_common.h"
#include <algorithm>
#define DEFAULTSERVER "sv"
#define CONTENTSZ 1024
using namespace std;
myshm server;
sem_t mutex;//simuler mutex
vector<char*> clients;
//3 operations

//connexion
void addClient(char* name){
    sem_wait(&mutex);
    clients.push_back(name);
    sem_post(&mutex);
}
//déconnexion
void delClient(char* name){
    sem_wait(&mutex);
    remove(clients.begin(),clients.end(),name);
    clients.pop_back();
    sem_post(&mutex);
}
//diffusion
void diffusion(char* message){
    for(char * client:clients){
        int fd=shm_open(client,O_CREAT|O_EXCL|O_RDWR,0600);
        ftruncate(fd,CONTENTSZ);
        void *mem=mmap(0,CONTENTSZ,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        char* m= new(mem) char();
        m=message;
        munmap(mem,CONTENTSZ);
        sem_unlink(client);
    }
}
int main(int argc,char** argv){
    sem_init(&mutex,-1,1);
    const char* server_ID=(argv[1]==0)?DEFAULTSERVER:argv[1];
    int msz=sizeof(struct message);
    cout<<server_ID<<endl;
    //creation espace partagé
    shm_unlink(server_ID);
    int fd= shm_open(server_ID,O_CREAT|O_RDWR|O_EXCL,0600);
    ftruncate(fd,msz);
    void * memoir=mmap(0,msz,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    message *msg=new(memoir) message;//msg dans une mémoire partagée
    //----
    //init semaphore
    sem_unlink(server_ID);
    server.sem= sem_open(server_ID,O_CREAT|O_EXCL|O_RDWR,0600,0);

    //1 connexion,
    //2 diffusion de message,
    //3 déconnexion ;
    //connexion
    while(true){
        sem_wait(server.sem);

        ++server.read;

        switch (msg->type) {
            case 1:

                addClient(msg->content);
                break;
            case 2:

                diffusion(msg->content);
                break;
            case 3:

                delClient(msg->content);
                break;
            default:

                cout<<"msg non matched"<<endl;
        }

    }

    return 0;


}