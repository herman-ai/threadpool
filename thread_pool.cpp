#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <queue>

using namespace std;


class thread_pool {

public:

    thread_pool() : nWaiting(N) { 
        for (int i=0; i < N; i++) {
            threads.push_back(thread(&thread_pool::looper, this));
        }
    }

    ~thread_pool() {
        for (auto & th : threads) {
            th.join();
        }
    }

    // get the number of running threads in the pool
    int size() { return static_cast<int>(this->threads.size()); }

    // number of idle threads
    int n_idle() { return this->nWaiting; }

    void push(function<void()> f) {
        tasks.push(f);
        cv.notify_one();
    }


private:

    const int N = 4;

    std::vector<std::thread> threads;
    queue<std::function<void()>> tasks;
    std::atomic<int> nWaiting;  // how many threads are waiting

    std::mutex m;
    std::condition_variable cv;

    void looper() {
    while (true) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this](){return !tasks.empty();});
        function<void()> task = tasks.front();
        task();
    }
}
};

void job() {
    cout << "Hello from job " << endl;
}


int main()
{
    thread_pool tp;
    tp.push(job);
}