#include "thread_pool.hpp"
#include <chrono>
#include <iostream>
#include <string>


using namespace std;

thread_pool::thread_pool() { 
    for (int i=0; i < N; i++) {
        threads.push_back(thread(&thread_pool::looper, this));
    }
}

thread_pool::~thread_pool() {
    for (auto & th : threads) {
        th.join();
    }
}

int thread_pool::push(function<void()> f) {
    tasks.push(f);
    cv.notify_one();
    return 0;
}

void thread_pool::looper() {
    while (true) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this](){return !tasks.empty();});
        function<void()> task = tasks.front();
        tasks.pop();
        task();
        completed_jobs.insert(task);
    }
}