//
// Created by Changrui on 25/11/22.
//
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#define COMMANDSIZE 10
#define BUFFERSIZE 9000
using namespace std;
int main(int argc,char ** argv){
    //iterer argv pour trouver les 2 commandes à executer
    char *command1[COMMANDSIZE];
    char *command2[COMMANDSIZE];
    int i=1;
    int pos=0;

    for(i;i<argc;i++){
        if(strcmp(argv[i],"|")==0){
            i++;
            command1[pos]= nullptr;
            break;
        }
        command1[pos]=argv[i];
        pos++;
    }
    pos=0;
    for(i;i<argc;i++){
        command2[pos]=argv[i];
        pos++;
    }

    int p[2];
    if (pipe(p) < 0)
        perror("error");

    int pid=fork();
    if(pid==-1){
        perror("fork");
    }else if(pid==0){
        close(p[0]);
        dup2(p[1],1);
        execv(command1[0],command1);
    }else{

        close(p[1]);
        char buffer[BUFFERSIZE];
        int sz=read(p[0],buffer,BUFFERSIZE);
        int fd=open("tmp.txt",O_CREAT|O_TRUNC|O_RDWR,0600);

        write(fd,buffer,sz);

        command2[pos]= (char*)"tmp.txt";
        command2[pos+1]= (char*)nullptr;
        execv(command2[0],command2);
        wait(nullptr);
        //cout<<buffer<<endl;
    }

}