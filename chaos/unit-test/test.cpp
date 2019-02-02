//
// Created by 关鑫 on 2019-02-01.
//

#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

std::timed_mutex test_mutex;

void f()
{
    test_mutex.try_lock_for(std::chrono::seconds(10));
    std::cout << "hello world\n";
}

int main()
{
    std::lock_guard<std::timed_mutex> l(test_mutex);
    std::thread t(f);
    t.join();
    return 0;
}