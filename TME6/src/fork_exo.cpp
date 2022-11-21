#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#define REPOS_MAX 1.0
#define REPOS_MIN 0.3

//probleme, n'arrive pas a entrer dans handler, kill sig tjrs 0?
using namespace std;
int vie=3;
void repos(){
    double alea=(double)((rand()%8)+3)/10;
    sleep(alea);
}
void handler(int signal){
	--vie;
    cout<<getpid()<<" handler! " <<vie<<endl;
}
int attaque(pid_t adversaire){
    //cout<<"attaque!"<<endl;
	signal(SIGINT,handler);
    return kill(adversaire,SIGINT);
}
void defense(){
    cout<<getpid()<<" defense!"<<endl;
    signal(SIGINT, SIG_IGN);
}
int combat(pid_t adversaire){
	while(vie>0){
        cout<<getpid()<<": combat vie: "<<vie<<endl;
        repos();
        int killsig=attaque(adversaire);
        cout<<"kill: "<<killsig<<endl;
        if(killsig==-1){
            cout<<getpid()<<" a gagné, "<<adversaire<<" a perdu!";
            return 0;
        }
        repos();
        defense();
	}
    cout<<adversaire<<" a gagné, "<<getpid()<<" a perdu!";
    return 1;
}

int main () {
	srand(time(nullptr));
    pid_t vador=getpid();
    pid_t luke=fork();
    signal(SIGINT,SIG_IGN);
    if(luke==-1){
        perror("luke");
        exit(1);
    }else if(luke==0){
        cout<<"luke"<<endl;
        int val=combat(vador);
        exit(val);
    }else{
        cout<<"vador: "<<vador<<"luke: "<<luke<<endl;
        int val= combat(luke);
        //wait(nullptr);
        exit(val);
    }
}
