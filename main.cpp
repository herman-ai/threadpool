#include "thread_pool.hpp"
#include <iostream>

using namespace std;

void job() {
    cout << "Hello from job " << endl;
}


int main()
{
    thread_pool tp;
    // Submit a job to thread pool
    tp.push(job);
    
    // Find out if the job was completed

}