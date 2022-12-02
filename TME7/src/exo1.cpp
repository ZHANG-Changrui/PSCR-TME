//
// Created by Changrui on 25/11/22.
//
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define COMMANDSIZE 10
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
    command2[pos]= nullptr;


    int output=dup(STDOUT_FILENO);
    int p[2];
    if (pipe(p) < 0)
        perror("error");

    cout<<"lol1"<<endl;
    dup2(p[1],STDOUT_FILENO);
    //char buffer[100];
    execv("/bin/ls",command1);
    //read(p[0],buffer,12);
    dup2(output,STDOUT_FILENO);
    //cout<<"lol"<<endl;
    cout<<"lol ca marche pas"<<endl;
    cout<<"afteer"<<endl;
}