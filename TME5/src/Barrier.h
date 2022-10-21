#pragma once
#include <mutex>
#include <condition_variable>
class Barrier{
	std::mutex m;
	int compteur;
	int N;
	std::condition_variable cv;
public:
	Barrier(int N):N(N),compteur(0){}
	void waitFor(){
		std::unique_lock<std::mutex> l(m);
		while(compteur>N){
			cv.wait(l);
		}
	}
	void done(){
		std::unique_lock<std::mutex> l(m);
		++compteur;
		if(compteur==N){
			cv.notify_all();
		}
	}

};
