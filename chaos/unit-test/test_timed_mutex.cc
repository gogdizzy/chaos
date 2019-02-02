#undef _GLIBCXX_USE_CLOCK_MONOTONIC

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

#include "boost/thread/mutex.hpp"
 
std::mutex cout_mutex; // control access to std::cout
std::timed_mutex mutex;
 
void job(int id) 
{
    using Ms = std::chrono::milliseconds;
    std::string s;
 
    for (int i = 0; i < 3; ++i) {
        if (mutex.try_lock_for(Ms(100))) {
            s += "success ";
            std::this_thread::sleep_for(Ms(100));
            mutex.unlock();
        } else {
            s += "failed ";
        }
        std::this_thread::sleep_for(Ms(100));
    }
 
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "[" << id << "] " << s << "\n";
}
 
int main() 
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(job, i);
    }
 
    for (auto& i: threads) {
        i.join();
    }
}

/*
int main() {
    std::timed_mutex mtx;
    using Ms = std::chrono::milliseconds;
    std::cout << mtx.try_lock_for(Ms(1000)) << '\n';
    return 0;
}
*/
