#pragma once

#include <cstring> // size_t,memset
#include <mutex>
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;

public :
    sem_t pop_sem;
    sem_t push_sem;
    sem_t mutex_sem;
	Stack () : sz(0) { memset(tab,0,sizeof tab) ;}
	T pop () {
        //std::unique_lock<std::mutex> l(m);
		// bloquer si vide
        sem_wait(&pop_sem);
        sem_wait(&mutex_sem);
		T toret = tab[--sz];
        sem_post(&push_sem);
        sem_post(&mutex_sem);
		return toret;
	}
    int size(){
        //std::unique_lock<std::mutex> l(m);
       return sz;
    }
	void push(T elt) {
        //std::unique_lock<std::mutex> l(m);
		//bloquer si plein
        sem_wait(&push_sem);
        sem_wait(&mutex_sem);
		tab[sz++] = elt;
        sem_post(&pop_sem);
        sem_post(&mutex_sem);
	}
};

}
