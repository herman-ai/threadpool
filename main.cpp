#include "thread_pool.hpp"
#include <iostream>
#include <chrono>


using namespace std;

void job() {
    cout << "Hello from job " << endl;
}


int main()
{
    thread_pool tp;
    // Submit a job to thread pool
    string id = tp.push(job);

    auto duration = std::chrono::seconds(5);
    std::this_thread::sleep_for(duration);

    cout << "Is the job done? " << tp.is_done(id) << endl;
    // Find out if the job was completed

}