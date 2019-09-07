// #ifndef _THREAD_POOL_HPP
// #define _THREAD_POOL_HPP

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <set>

using std::thread;
using std::function;
using std::vector;
using std::queue;
using std::set;

class thread_pool {

public:

    thread_pool();
    ~thread_pool();

    int push(function<void()> f);

private:

    const int N = 4;

    set<function<void()>> completed_jobs;

    vector<thread> threads;
    queue<function<void()>> tasks;

    std::mutex m;
    std::condition_variable cv;

    void looper();
};

// #endif