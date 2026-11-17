#include <coroutine>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <iomanip>

using namespace std;

// =========================================================================
// ПУНКТ 1: ПРИМЕР ИЗ ЗАДАНИЯ (выводит твое число)
// =========================================================================
struct promise_1;
struct coroutine : std::coroutine_handle<promise_1> {
    using promise_type = ::promise_1;
};

struct promise_1 {
    coroutine get_return_object() { return { coroutine::from_promise(*this) }; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
};

void run_task_1() {
    cout << "--- Пункт 1: Базовый пример ---" << endl;

    // Задаем СВОЕ число (например, 2026)
    coroutine h = [](int i) -> coroutine {
        std::cout << "Мое число: " << i << std::endl;
        co_return;
        }(2026);

    h.resume();
    h.destroy();
    cout << endl;
}


// =========================================================================
// ПУНКТ 2: ПРОГРЕСС-БАР С ТВОИМ ИМЕНЕМ
// =========================================================================
struct promise_type_2 {
    int current_value = 0;
    auto get_return_object() { return std::coroutine_handle<promise_type_2>::from_promise(*this); }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
    std::suspend_always yield_value(int value) {
        current_value = value;
        return {};
    }
};

struct task {
    std::coroutine_handle<promise_type_2> handle;
    task(std::coroutine_handle<promise_type_2> h) : handle(h) {}
    ~task() { if (handle) handle.destroy(); }
    void resume() { handle.resume(); }
    bool done() const { return handle.done(); }
    int get_value() const { return handle.promise().current_value; }
};

namespace std {
    template<>
    struct coroutine_traits<task, int> {
        using promise_type = ::promise_type_2;
    };
}

// Сама корутина
task long_computation(int steps) {
    for (int i = 0; i < steps; ++i) {
        this_thread::sleep_for(chrono::milliseconds(10)); // Имитация работы
        co_yield i + 1; // Отдаем текущий шаг
    }
}

// Отрисовка прогресс-бара с твоим именем
void print_progress(int current, int total) {
    float percent = static_cast<float>(current) / total * 100.f;
    int bar_width = 50;
    string bar(bar_width, ' ');

    size_t pos = static_cast<size_t>(percent / 100.f * bar_width);

    // ТВОЕ ИМЯ ЗДЕСЬ (латинскими буквами, чтобы не было проблем с кодировкой)
    string name = "ANNA";

    for (size_t i = 0; i < pos; ++i) {
        bar[i] = name[i % name.length()];
    }

    cout << "\r[" << bar << "] " << fixed << setprecision(1) << percent << "%";
    cout.flush();
}

void run_task_2() {
    cout << "--- Пункт 2: Прогресс-бар с именем ---" << endl;
    const int TOTAL_STEPS = 100;
    auto coro = long_computation(TOTAL_STEPS);

    while (!coro.done()) {
        coro.resume();
        int progress = coro.get_value();
        print_progress(progress, TOTAL_STEPS);
        this_thread::yield();
    }
    cout << "\nDONE!" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    run_task_1();
    run_task_2();

    system("pause");
    return 0;
}