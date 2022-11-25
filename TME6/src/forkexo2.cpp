#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
int main () {
    /*
     * Question 1:En comptant le processus initial, combien de processus engendre l'execution du programme ?
     * Donnez l'arbre des descendances de processus, et l'achage de chacun d'entre eux.
        Il y a 10 processus sont crées:
    0-> 0 -> 0 -> 0
        |    |\   |\ \
        v    v v  v v v
        0    0 0  0 0 0
     */
    const int N = 3;
    std::cout << "main pid=" << getpid() << std::endl;
    int fils=0;
    for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {

        std::cout << " i:j " << i << ":" << j << std::endl;
        for (int k=1; k<=i && j==N ; k++) {
            if ( fork() == 0) {
                j=0;
                std::cout << " k:j " << k << ":" << j << std::endl;
            }else{
                ++fils;
            }
        }
        //std::cout<<"nbr fils:"<<fils<<std::endl;
    }
    for(int j=0;j<fils;j++){
        wait(nullptr);
    }
    return 0;
}