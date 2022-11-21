#include <iostream>
#include <unistd.h>
#include <wait.h>
using namespace std;
int vie=3;

void handler(){
	--vie;
}
void attaque(pid_t adversaire){
	struct sigaction act;
	act.sa_handler=handler;
	sigaction(SIGINT,&act,nullptr);
	kill(adversaire,SIGINT);
}
void defense(){
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig,SIGINT);

}
void combat(pid_t adversaire){
	while(1){

	}
}

int main () {
	srand(time(nullptr));
	/*
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	int nbrfils=0;
	for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			pid_t pid=fork();	int vie=3;
	int pid=fork();
	int sleepTime=(rand()%0.6)+0.3;
	if(pid==-1){
		perror("error fork");
		exit(1);
	}else if(pid==0){
		//fils TODO
		while(vie){
			defense();
			sleep(sleepTime);
		}
	}else{
		//Pere TODO
		while(vie){
			sleep(sleepTime);
		}
	}
			if ( pid == 0) {
				nbrfils=0;
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
				cout<<"je suis :"<<getpid()<<endl;
			}else{
				nbrfils++;
				int status;
				wait(&status);
				cout<<" je suit "<<getpid()<<" mon fils se termine. fils: "<<pid<<" "<<endl;
			}
		}
	}*/

	int pid=fork();
	//  double sleepTime=(rand()%0.6)+0.3;
	if(pid==-1){
		perror("error fork");
		exit(1);
	}else if(pid==0){
		//fils TODO
		while(vie){
			combat(getppid());
		}
	}else{
		//Pere TODO
		while(vie){
			combat(pid);
		}
	}


	return 0;
}
