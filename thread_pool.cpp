#include "thread_pool.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

thread_pool::thread_pool() : stop(false) {
    for (int i=0; i < N; i++) {
        threads.push_back(thread(&thread_pool::looper, this));
    }
}

thread_pool::~thread_pool() {
    for (auto & th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

string  unique_id() {
    // generate and eight character long unique identifier
    
    string s = "";
    for (int i=0; i< 8; i++) {
        int r = random() % 26;
        s += char('a'+r);
    }
    return s;
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
        cv.wait(lock, [this](){return (!tasks.empty() || stop);});
        if (stop) {
            cout << "Child Thread:: need to end this now" << endl;
            break;
        }
        pair<string, function<void()>> id_task = tasks.front();
        tasks.pop();
        active_threads += 1;
        id_task.second(); // execute the task
        active_threads -= 1;
        completed_jobs.insert(id_task.first);
    }
}

void thread_pool::terminate() {
    {
        lock_guard<mutex> lock(m);
        //first kill all jobs that are still in queue
        while (!tasks.empty()) {
            tasks.pop();
        }
        cout << "Main thread :: Task queue is empty now" << endl;
        //need to wait for any tasks that are in progress

        stop = true;
        cv.notify_all();        
    }
    
    for (auto & th : threads) {
        cout << "Main Thread :: Checking if the thread is joinable" << endl;
        if (th.joinable()) {
            cout << "Main thread :: Thread is joinable" << endl;
            th.join();
        } else {
            cout << "Main Thread :: Thread is not joinable" << endl;
        }
    }
    cout << "Main Thread :: All active jobs have finished." << endl;
    cout << "Main Thread : Thread pool is going away!" << endl;
}
