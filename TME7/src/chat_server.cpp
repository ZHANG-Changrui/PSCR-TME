#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include "chat_common.h"
#include <algorithm>
#define DEFAULTSERVER "sv"
#define CONTENTSZ 1024
#define MAXCLIENT 10
using namespace std;
myshm server;
sem_t mutex;//simuler mutex
char *clients[MAXCLIENT];
int nbr_client=0;
struct message *msg;

//diffusion
void diffusion(){
    cout<<"message à diffuser: "<<msg->content<<endl;
    for(int i=0;i<MAXCLIENT;i++){
        if(clients[i]!=0){
            //cout<<"diffusion ce client: "<<clients[0]<<endl;
            int fd=shm_open(clients[i],O_RDWR,0600);
            ftruncate(fd,CONTENTSZ);
            void *mem=mmap(0,CONTENTSZ,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
            char* content= new(mem) char();
            //char* content=(char*)mem;
            strcpy(content,msg->content);

            munmap(mem,CONTENTSZ);
            close(fd);
            sem_t *client_sem= sem_open(clients[i],O_RDWR,0600,0);
            sem_post(client_sem);
            sem_close(client_sem);
        }
    }
}
int main(int argc,char** argv){
    sem_init(&mutex,-1,1);
    const char* server_ID=(argv[1]==0)?DEFAULTSERVER:argv[1];
    int msz=sizeof(struct message);
    cout<<"server: "<<server_ID<<endl;
    //creation espace partagé
    shm_unlink(server_ID);
    int fd= shm_open(server_ID,O_CREAT|O_RDWR|O_EXCL,0600);
    ftruncate(fd,msz);
    void * memoir=mmap(0,msz,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    msg=new(memoir)struct message;//msg dans une mémoire partagée
    //----
    //init semaphore
    sem_unlink(server_ID);
    server.sem= sem_open(server_ID,O_CREAT|O_EXCL|O_RDWR,0600,0);
    msg->type=122;
    //1 connexion,
    //2 diffusion de message,
    //3 déconnexion ;
    //connexion
    while(true){
        sem_wait(server.sem);
        ++server.read;
        cout<<"traitement type:"<<msg->type<<endl;
        if(msg->type==1){//connexion
            sem_wait(&mutex);
            for(int i=0;i<MAXCLIENT;i++){
                if(clients[i]==0){
                    clients[i]=new char[strlen(msg->content)];
                    strcpy(clients[i],msg->content);
                    ++nbr_client;
                    break;
                }
            }
            sem_post(&mutex);
        }else if(msg->type==2){//diffusioin
            diffusion();
        }else if(msg->type==3){//deconnexion
            sem_wait(&mutex);
            //free(clients[nbr_client]);
            --nbr_client;
            for(int i=0;i<MAXCLIENT;i++){
                if(clients[i]!=0&&(strcmp(clients[i],msg->content)==0)){
                    clients[i]=0;
                    delete clients[i];
                }
            }
            sem_post(&mutex);
        }else{
            cout<<"autre"<<endl;
        }
        for(int i=0;i<nbr_client;i++){
            cout<<"client: "<<clients[i]<<endl;
        }
    }

    return 0;


}