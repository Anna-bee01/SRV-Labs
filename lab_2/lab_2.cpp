#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>

using namespace std;

mutex m;

void Func(string name) {
    long double i = 0;

    auto start_time = chrono::steady_clock::now();

    while (true) {
        auto current_time = chrono::steady_clock::now();
        chrono::duration<double> elapsed = current_time - start_time;

        if (elapsed.count() >= 1.0) {
            break;
        }

        i += 1e-9;
    }

    m.lock();
    cout << name << ": " << i << endl;
    m.unlock();
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Запуск потоков..." << endl;

    thread thread1(Func, "t1");
    thread thread2(Func, "t2");
    thread thread3(Func, "t3");

    thread1.join();
    thread2.join();
    thread3.join();

    cout << "Все потоки завершены." << endl;
    system("pause");
    return 0;
}