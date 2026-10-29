#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;
using namespace std::chrono;


long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

void factor_10() {
    cout << "Факториал 10! - 10 млн раз." << endl;

    clock_t start = clock();

    long long res = 0;
    for (int i = 0; i < 10000000; ++i) {
        res = factorial(10);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    cout << "Результат: " << res << endl;
    cout << "Время выполнения (clock): " << seconds << " сек." << endl << endl;
}


void Func(string name) {
    long long res = 0;
    for (int i = 0; i < 10000000; ++i) {
        res = factorial(10);
    }
    cout << "Поток " << name << " завершил работу. Результат: " << res << endl;
}

void multi_threaded() {
    cout << "Многопоточность" << endl << endl;

    cout << "Запуск последовательно (2 раза)" << endl;
    auto start_seq = high_resolution_clock::now();

    Func("t1_seq");
    Func("t2_seq");

    auto end_seq = high_resolution_clock::now();
    duration<double> time_seq = end_seq - start_seq;
    cout << "Общее время (последовательно): " << time_seq.count() << " сек." << endl << endl;

    cout << "Запуск параллельно (в 2 потоках)" << endl;
    auto start_par = high_resolution_clock::now();

    thread thread1(Func, "t1");
    thread thread2(Func, "t2");

    thread1.join();
    thread2.join();

    auto end_par = high_resolution_clock::now();
    duration<double> time_par = end_par - start_par;
    cout << "Общее время (параллельно): " << time_par.count() << " сек." << endl << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    factor_10();

    multi_threaded();

    system("pause");
    return 0;
}