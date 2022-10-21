#include "Banque.h"
#include <time.h>
using namespace std;

const int NB_THREAD =1000;
void trans(pr::Banque &b){
        int i=rand()%(b.size());
        int j=rand()%(b.size());
        int m=rand()%101;
        b.transfert(i,j,m);
        int tsleep=rand()%21;
        this_thread::sleep_for(std::chrono::milliseconds(tsleep));
}
void comptabiliser(pr::Banque &b,int n){
    int tsleep=rand()%21000;
    this_thread::sleep_for(std::chrono::milliseconds(tsleep));
    int eg=b.comptabiliser(n);
    if(eg){
        cout<<"OK cpt"<<endl;
    }else{
        cout<<"NON cpt"<<endl;
    }
}
int main () {
    pr::Banque b(20,100);
    srand(time(NULL));
	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé

    for(int i=0;i<NB_THREAD;i++){
        threads.emplace_back(comptabiliser,std::ref(b),20*100);
        threads.emplace_back(trans,std::ref(b));


    }


	for (auto & t : threads) {
		t.join();
	}
    b.afficheList();

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
