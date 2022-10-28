#include <iostream>
#include <unistd.h>
#include <wait.h>
using namespace std;
int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	int nbrfils=0;
	for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			pid_t pid=fork();
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
	}
	return 0;
}
