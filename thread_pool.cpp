#include "thread_pool.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>


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

string  unique_id() {
    return "abc";
}

string thread_pool::push(function<void()> f) {
    string id = unique_id();
    pair<string, function<void()>> id_task(id, f);
    tasks.push(id_task);
    cv.notify_one();
    return id;
}

bool thread_pool::is_done(string id) {
    if (find(completed_jobs.begin(), completed_jobs.end(), id)!=completed_jobs.end()) {
        return true;
    }
    return false;
}

void thread_pool::looper() {
    while (true) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this](){return !tasks.empty();});
        pair<string, function<void()>> id_task = tasks.front();
        tasks.pop();
        id_task.second();
        completed_jobs.insert(id_task.first);
    }
}
