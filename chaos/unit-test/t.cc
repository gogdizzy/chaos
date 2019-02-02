//
// Created by 关鑫 on 2019-02-01.
//
/**
 * 设计思路：
 * 主要是对比std::timed_mutex::try_lock_for是否真的超时才失败
 * 需要3个线程，其中2个在等待，1个触发，
 * 如果结果正确，那么应该只有1个被激活，另1个还在等待
 * 如果结果错误，那么应该1个激活，另1个返回错误
 */

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

//#include "boost/thread/mutex.hpp"

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;

template < class Duration >
using TimePoint = std::chrono::time_point<Clock, Duration>;

template < typename Duration >
void printTimepoint( const std::string &tag, const TimePoint< Duration > &timePoint ) {
    std::cout << tag << timePoint.time_since_epoch().count() << '\n';
}

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

void testStd() {
    std::cout << "=== Std Begin ===" << '\n';

    printTimepoint( "Start: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );

    std::timed_mutex mtx;

    auto consumer = [&]() {
        std::this_thread::sleep_for( Ms( 100 ) );
        auto rv = mtx.try_lock_for( Ms( 300 ) );
        std::cout << std::this_thread::get_id() << " return " << rv << " : ";
        printTimepoint( "", std::chrono::time_point_cast<Ms>( Clock::now() ) );
        if( rv ) {
            std::this_thread::sleep_for( Ms( 100 ) );
            mtx.unlock();
        }
    };

    auto producer = [&]() {
        mtx.lock();
        printTimepoint( "Producer lock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
        std::this_thread::sleep_for( Ms( 200 ) );
        printTimepoint( "Producer unlock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
        mtx.unlock();
    };

    std::thread t1( producer );
    std::thread t2( consumer );
    std::thread t3( consumer );

    t1.join();
    t2.join();
    t3.join();

    std::cout << "=== Std End ===" << '\n';
}

void testBoost() {

}

int main()
{
//    printTimepoint( "Start: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//    std::vector<std::thread> threads;
//    for (int i = 0; i < 4; ++i) {
//        threads.emplace_back(job, i);
//    }
//
//    for (auto& i: threads) {
//        i.join();
//    }
    testStd();
}


