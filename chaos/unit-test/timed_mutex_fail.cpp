//
// Created by 关鑫 on 2019-02-01.
//

// fail with gcc 4.8

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

//#include <iostream>
//#include <string>
//#include <mutex>
//#include <thread>
//#include <vector>
//
//using Clock = std::chrono::high_resolution_clock;
//using Ms = std::chrono::milliseconds;
//using Sec = std::chrono::seconds;
//
//template < class Duration >
//using TimePoint = std::chrono::time_point<Clock, Duration>;
//
//template < typename Duration >
//void printTimepoint( const std::string &tag, const TimePoint< Duration > &timePoint ) {
//    std::cout << tag << timePoint.time_since_epoch().count() << '\n';
//}
//
//std::mutex cout_mutex; // control access to std::cout
//std::timed_mutex mutex;
//
//void job(int id)
//{
//    using Ms = std::chrono::milliseconds;
//    std::string s;
//
//    for (int i = 0; i < 3; ++i) {
//        if (mutex.try_lock_for(Ms(100))) {
//            s += "success ";
//            std::this_thread::sleep_for(Ms(100));
//            mutex.unlock();
//        } else {
//            s += "failed ";
//        }
//        std::this_thread::sleep_for(Ms(100));
//    }
//
//    std::lock_guard<std::mutex> lock(cout_mutex);
//    std::cout << "[" << id << "] " << s << "\n";
//}
//
//void testStd() {
//    std::cout << "=== Std Begin ===" << '\n';
//
//    printTimepoint( "Start: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//
//    std::timed_mutex mtx;
//
//    auto consumer = [&]() {
//        std::this_thread::sleep_for( Ms( 100 ) );
//        auto rv = mtx.try_lock_for( Ms( 300 ) );
//        std::cout << std::this_thread::get_id() << " return " << rv << " : ";
//        printTimepoint( "", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//        if( rv ) {
//            printTimepoint( "Consumer lock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//            std::this_thread::sleep_for( Ms( 100 ) );
//            printTimepoint( "Consumer unlock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//            mtx.unlock();
//        }
//    };
//
//    auto producer = [&]() {
//        mtx.lock();
//        printTimepoint( "Producer lock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//        std::this_thread::sleep_for( Ms( 200 ) );
//        printTimepoint( "Producer unlock: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
//        mtx.unlock();
//    };
//
//    std::thread t1( producer );
//    std::thread t2( consumer );
//    std::thread t3( consumer );
//
//    t1.join();
//    t2.join();
//    t3.join();
//
//    std::cout << "=== Std End ===" << '\n';
//}
//
//void testBoost() {
//
//}
//
//int main()
//{
////    printTimepoint( "Start: ", std::chrono::time_point_cast<Ms>( Clock::now() ) );
////    std::vector<std::thread> threads;
////    for (int i = 0; i < 4; ++i) {
////        threads.emplace_back(job, i);
////    }
////
////    for (auto& i: threads) {
////        i.join();
////    }
//    testStd();
//}

