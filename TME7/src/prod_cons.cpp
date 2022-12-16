#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int semaphoreAnonymous=false;
char* shmaddr=(char*)"/myshm";
Stack<char> *s;
void handler(int signale){
    if(!semaphoreAnonymous){
        shm_unlink(shmaddr);
        return ;
    }else{
        return;
    }
}
int main () {
    void *mem;
    if(!semaphoreAnonymous){
        shm_unlink(shmaddr);
        int fd = shm_open(shmaddr, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd == -1) {perror("open error");}
        if (ftruncate(fd, sizeof(Stack<char>))== -1) {perror("error trunc");}
        mem = mmap(0, sizeof(Stack<char>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    }else{
        mem = (Stack<char> *) mmap(0, sizeof(Stack<char>), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    }

    if (mem == MAP_FAILED){
        perror("mmap");
    }

    s = new (mem) Stack<char>();

    sem_init(&s->mutex_sem,1,1);
    sem_init(&s->pop_sem,1,0);
    sem_init(&s->push_sem,1,1);

	pid_t pp = fork();
	if (pp==0) {
        signal(SIGINT,handler);
		producteur(s);
		return 0;
	}
	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}
    wait(0);
    wait(0);
	delete s;
	return 0;
}

