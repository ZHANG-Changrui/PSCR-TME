#include "Pool.h"
#include <istream>
using namespace std;
using namespace pr;


void PoolWorker(Queue<pr::Job> &queue){
    while(true){
        Job *j=queue.pop();
        if(j== nullptr){
            return;
        }
        j->run();
        delete j;
    }
}
Pool::Pool(int qsize):queue(qsize){}
void Pool::start(int nbthread) {
    for(int i=0;i<nbthread;i++){
        threads.emplace_back(PoolWorker,std::ref(queue));
    }
}
void Pool::submit(pr::Job *job) {
    if(job!= nullptr){
        queue.push(job);
    }
}
void Pool::stop() {
    queue.setBlocking(false);
}
Pool::~Pool() {}


