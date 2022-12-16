#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include "chat_common.h"
#include <algorithm>
#include <thread>
//TODO fin propre
#define CONTENTSZ 1024
#define DEFAULTSERVER "sv"
#define DEFAULTCLIENT "cl"
using namespace std;
bool running=true;
struct message *msg;
char* content;
sem_t *sem;
sem_t *mysem;
const char* client_ID;
const char* server_ID;
int fd;
int fdContent;
void * mem2;
vector<thread> threads;
struct message *connexion;
struct message *deconnexion;
void send(struct message *m){
    msg->type=m->type;
    strcpy(msg->content,m->content);
    //cout<<"send type "<<msg->type<<endl;
    sem_post(sem);
}
void lire(){
    sem_unlink(client_ID);
    mysem= sem_open(client_ID,O_CREAT|O_EXCL|O_RDWR,0600,0);

    //memoir partagé reception message
    shm_unlink(client_ID);
    fdContent= shm_open(client_ID,O_CREAT|O_EXCL|O_RDWR,0600);
    ftruncate(fdContent,CONTENTSZ);
    mem2=mmap(0,CONTENTSZ,PROT_WRITE|PROT_READ,MAP_SHARED,fdContent,0);
    content=new(mem2)char();

    while(running){
        sem_wait(mysem);
        if(!running){
            break;
        }
        cout<<"message diffusé: "<<content<<endl;
    }
}

void ecrire(){
    struct message *message;
    message->type=2;//diffusion

    while (running){
        cin.getline(message->content,CONTENTSZ);
        //cin>>message->content;
        if(!running){
            break;
        }
        send(message);
    }
}
bool SIGINTACT=false;
void end(int signal){
    if(!SIGINTACT){
        running= false;
        sem_post(mysem);
        close(fd);
        close(fdContent);
        cin.eof();
        send(deconnexion);
        shm_unlink(client_ID);
        sem_unlink(client_ID);
        munmap(mem2,sizeof(struct message));
        SIGINTACT= true;
    }

}

int main(int argc,char**argv){
    //handler
    signal(SIGINT,end);
    client_ID=(argc>1)?argv[1]:DEFAULTCLIENT;
    server_ID=(argc>2)?argv[2]:DEFAULTSERVER;
    cout<<"client: "<<client_ID<<endl;
    cout<<"server: "<<server_ID<<endl;
    //connexion

    connexion=new struct message;
    connexion->type=1;
    strcpy(connexion->content,client_ID);
    //deconnexion

    deconnexion=new struct message;
    deconnexion->type=3;
    strcpy(deconnexion->content,client_ID);

    //--------------------------------------------------------------------------------server
    int msz=sizeof(struct message);
    //creation memoir partagé
    //memoire partagé du server
    fd= shm_open(server_ID,O_RDWR,0600);
    ftruncate(fd,msz);
    void * memoir=mmap(0,msz,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    msg=new(memoir)struct message;//msg dans une mémoire partagée
    //msg=(struct message*)mempor;
    //sem
    sem= sem_open(server_ID,O_RDWR,0600,0);
    //sem_post(sem);//TODO
    //--------------------------------------------------------------------------------client




    send(connexion);

    threads.emplace_back(lire);
    threads.emplace_back(ecrire);
    for(auto &t:threads){
        t.join();
    }
    delete connexion;
    delete deconnexion;
}