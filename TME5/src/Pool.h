#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize){
		queue=Queue<Job>(qsize);
	}


	void start (int nbthread){
		for(int i=0;i<nbthread;i++){
			threads.emplace_back(queue.pop());
		}
	}
	void submit (Job * job){
		queue.push(job);
	}
	void stop(){
		queue.setBlocking(false);
	}
	void poolWorker(Queue<Job> &queue){
		while(1){
			Job *j=queue.pop();
			if(j==nullptr){
				break;
			}
			j->run();
			delete j;
		}
	}
	~Pool(){
		delete queue;
		delete[] threads;
	}
};

}
