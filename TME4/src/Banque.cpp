#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
    debiteur.getMutex().lock();
    crediteur.getMutex().lock();
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
    debiteur.getMutex().unlock();
    crediteur.getMutex().unlock();
}
size_t Banque::size() const {
	return comptes.size();
}
void Banque::affiche() {
    int i=0;
    for(auto c:comptes){
        cout<<i<<" : "<<c.getSolde()<<endl;
        i++;
    }
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
