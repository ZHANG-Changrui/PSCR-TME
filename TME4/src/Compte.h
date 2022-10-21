#pragma once
#include <thread>
#include <mutex>
#include <atomic>
namespace pr {

class Compte {
	mutable std::recursive_mutex m;
	std::atomic<int> solde;
public :
	Compte(int solde=0):solde(solde) {}
	Compte(const Compte & other);
	void crediter (unsigned int val) ;
	bool debiter (unsigned int val) ;
	int getSolde() const  ;
    std::recursive_mutex & getMutex();
};

}
