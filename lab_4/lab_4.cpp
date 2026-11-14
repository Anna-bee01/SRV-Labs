#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

int gluttony = 1;           // Сколько съедает толстяк за раз
int efficiency_factor = 1;  // Сколько кладет Кук за раз на каждую тарелку
int dish1 = 3000, dish2 = 3000, dish3 = 3000;
int eaten1 = 0, eaten2 = 0, eaten3 = 0;

bool game_over = false;

string result_message = "";

mutex mtx;

chrono::steady_clock::time_point global_start_time; // Глобальное время старта

// Функция для сброса состояния перед каждым тестом
void reset_state() {
    dish1 = 3000; dish2 = 3000; dish3 = 3000;
    eaten1 = 0; eaten2 = 0; eaten3 = 0;
    game_over = false;
    result_message = "";
    global_start_time = chrono::steady_clock::now();
}

// Функция для толстяка
void fat_man(int id, int& dish, int& eaten) {
    while (!game_over) {
        if (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - global_start_time).count() >= 5) {
            mtx.lock();
            if (!game_over) {
                game_over = true;
                result_message = "Кук уволился сам (прошло 5 дней)";
            }
            mtx.unlock();
            break;
        }

        mtx.lock();

        if (eaten >= 10000) {
            mtx.unlock();
            break;
        }

        if (dish > 0) {
            if (dish - gluttony < 0 && !game_over) {
                game_over = true;
                result_message = "Кука уволили (тарелка опустела)";
            }
            dish -= gluttony;
            eaten += gluttony;
        }
        else {
            this_thread::yield();
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(10)); // Задержка
    }
}

// Функция для Кука
void cook() {
    while (!game_over) {
        if (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - global_start_time).count() >= 5) {
            mtx.lock();
            if (!game_over) {
                game_over = true;
                result_message = "Кук уволился сам (прошло 5 дней)";
            }
            mtx.unlock();
            break;
        }

        mtx.lock();

        dish1 += efficiency_factor;
        dish2 += efficiency_factor;
        dish3 += efficiency_factor;

        mtx.unlock();

        this_thread::sleep_for(chrono::microseconds(10)); // Задержка
    }
}

void run_test(string test_name, int glut, int eff) {
    reset_state();
    gluttony = glut;
    efficiency_factor = eff;

    cout << endl << endl;
    cout << "Тест: " << test_name << endl;
    cout << "Параметры: Толстяк ест: " << glut << ", Кук кладет: " << eff << endl;

    thread t1(fat_man, 1, ref(dish1), ref(eaten1));
    thread t2(fat_man, 2, ref(dish2), ref(eaten2));
    thread t3(fat_man, 3, ref(dish3), ref(eaten3));
    thread t_cook(cook);

    t1.join(); t2.join(); t3.join(); t_cook.join();

    cout << "Съедено: " << endl;
    cout << "Толстяк 1: " << eaten1 << endl;
    cout << "Толстяк 2: " << eaten2 << endl;
    cout << "Толстяк 3: " << eaten3 << endl << endl;

    if (eaten1 >= 10000 && eaten2 >= 10000 && eaten3 >= 10000) {
        cout << "Результат: Кук не получил зарплату (все толстяки самоуничтожились)" << endl;
    }
    else {
        cout << "Результат: " << result_message << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    run_test("Тест 1 (кука уволили)", 100, 1);

    run_test("Тест 2 (кук не получил зарплату)", 1000, 10000);

    run_test("Тест 3 (кук уволился)", 1, 5);

    system("pause");
    return 0;
}