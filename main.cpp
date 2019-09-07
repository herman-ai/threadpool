#include "thread_pool.hpp"
#include <iostream>
#include <chrono>


using namespace std;

void job() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    cout << "Child Thread: Hello from job " << endl;
}


int main()
{
    thread_pool tp;
    // Submit a job to thread pool

    cout << "Main Thread: Submitting a job" << endl;
    string id = tp.push(job);

    cout << " Main thread :: Is the job done? " << std::boolalpha << tp.is_done(id) << endl;
    
    //Send a stop signal to the thread pool
    tp.terminate();

}