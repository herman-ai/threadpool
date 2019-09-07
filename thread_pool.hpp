// #ifndef _THREAD_POOL_HPP
// #define _THREAD_POOL_HPP

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <set>
#include <string>

using std::thread;
using std::function;
using std::vector;
using std::queue;
using std::set;
using std::string;
using std::pair;

class thread_pool {

public:

    thread_pool();
    ~thread_pool();

    string push(function<void()> f);
    bool is_done(string);

private:

    const int N = 4;

    set<string> completed_jobs;

    vector<thread> threads;
    queue<pair<string, function<void()>>> tasks;

    std::mutex m;
    std::condition_variable cv;

    void looper();
};

// #endif