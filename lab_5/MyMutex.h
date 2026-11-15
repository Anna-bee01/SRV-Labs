#pragma once
#include <atomic>

class MyMutex {
private:
    std::atomic<bool> locked;

public:
    MyMutex();
    void lock();
    void unlock();
};