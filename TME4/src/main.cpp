#include "Banque.h"
#include <cstdlib>
#include <iostream>
using namespace std;

const int NB_THREAD = 2;

//Q1
void thread_trans(pr::Banque &b){
    int m=rand()%100;
    int i=rand()%(b.size());
    int j=rand()%(b.size());
    b.transfert(i,j,m);
    int tsleep=rand()%20;
    this_thread::sleep_for(std::chrono::milliseconds(tsleep));
}
int main () {
    //initialisation Banque
    pr::Banque b(10,1000);
    ::srand(::time((nullptr)));
	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
    for(int i=0;i<NB_THREAD;++i){
        threads.push_back(thread(thread_trans,std::ref(b)));
    }
	for (auto & t : threads) {
		t.join();
	}
    b.affiche();
	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
