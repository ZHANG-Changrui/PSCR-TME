#ifndef POOL_H
#define POOL_H

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {
    void PoolWorker(Queue<pr::Job> &queue){
        //std::cout<<"slol"<<std::endl;
        while(true){
            Job *j=queue.pop();
            if(j== nullptr){
                return;
            }
            j->run();
            delete j;
        }
    }
    class Pool {
        Queue<Job> queue;
        std::vector <std::thread> threads;
    public:
        Pool(int qsize):queue(qsize){}
        void start(int nbthread) {
            for(int i=0;i<nbthread;i++){
                threads.emplace_back(PoolWorker,std::ref(queue));
            }
        }
        void submit(pr::Job *job) {
            if(job!= nullptr){
                queue.push(job);
            }
        }
        void stop() {
            queue.setBlocking(false);
        }
        ~Pool(){};
    };

}
#endif