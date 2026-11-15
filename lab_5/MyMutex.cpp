#include "MyMutex.h"
#include <thread>

MyMutex::MyMutex() : locked(false) {}

void MyMutex::lock() {
    bool expected = false;
    // compare_exchange_weak пытается записать true в locked, 
    // если там было false. Если удалось — мы захватили мьютекс.
    while (!locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
        expected = false;
        std::this_thread::yield();
    }
}

void MyMutex::unlock() {
    locked.store(false, std::memory_order_release);
}