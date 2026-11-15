#include "MyMutex.h"
#include <thread>

MyMutex::MyMutex() : locked(false) {}

void MyMutex::lock() {
    bool expected = false;
    while (!locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
        expected = false;
        std::this_thread::yield();
    }
}

void MyMutex::unlock() {
    locked.store(false, std::memory_order_release);
}