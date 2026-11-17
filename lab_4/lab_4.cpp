#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

int gluttony = 1;           // Сколько съедает толстяк за раз
int efficiency_factor = 1;  // Сколько кладет Кук за раз на каждую тарелку
int waiters_ready = 0;      // Сколько толстяков сделали ход в текущем раунде
int alive_fat_men = 3;      // Сколько толстяков еще в игре (не вышли)

int dish1 = 3000, dish2 = 3000, dish3 = 3000;
int eaten1 = 0, eaten2 = 0, eaten3 = 0;

bool game_over = false;
bool cook_turn = true;      // true - ход Кука, false - ход толстяков

string result_message = "";

mutex mtx;

chrono::steady_clock::time_point global_start_time;

// Функция для сброса состояния перед каждым тестом
void reset_state() {
    dish1 = 3000; dish2 = 3000; dish3 = 3000;
    eaten1 = 0; eaten2 = 0; eaten3 = 0;
    game_over = false;
    result_message = "";
    waiters_ready = 0;
    alive_fat_men = 3;
    cook_turn = true;
    global_start_time = chrono::steady_clock::now();
}

// Завершаем игру досрочно, если все толстяки вышли
void check_all_fat_men_dead() {
    if (alive_fat_men <= 0 && !game_over) {
        game_over = true;
        if (eaten1 >= 10000 && eaten2 >= 10000 && eaten3 >= 10000) {
            result_message = "Кук не получил зарплату (все толстяки самоуничтожились)";
        }
        else {
            result_message = "Кука уволили (тарелка опустела)";
        }
    }
}

// Функция для толстяка
void fat_man(int id, int& dish, int& eaten) {
    while (!game_over) {
        // Проверка таймера
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

        // Если ход Кука — ждем
        if (cook_turn) {
            mtx.unlock();
            this_thread::yield();
            continue;
        }

        // Толстяк уже лопнул — просто отмечаемся и выходим
        if (eaten >= 10000) {
            waiters_ready++;
            if (waiters_ready >= 3) {
                waiters_ready = 0;
                cook_turn = true;
            }
            mtx.unlock();
            break;
        }

        // Толстяк ест
        if (dish >= gluttony) {
            dish -= gluttony;
            eaten += gluttony;
        }
        else {
            if (dish > 0) {
                eaten += dish;
                dish = 0;
            }
            // Тарелка опустела — Кука увольняют
            if (dish <= 0 && !game_over) {
                game_over = true;
                result_message = "Кука уволили (тарелка опустела)";
                waiters_ready++;
                alive_fat_men--;
                check_all_fat_men_dead();
                mtx.unlock();
                break;
            }
        }

        // Если толстяк лопнул после еды
        if (eaten >= 10000) {
            alive_fat_men--;
            check_all_fat_men_dead();
        }

        waiters_ready++;

        // Если все три сделали ход — передаем ход Куку
        if (waiters_ready >= 3) {
            waiters_ready = 0;
            cook_turn = true;
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(10)); // Задержка
    }
}

// Функция для Кука
void cook() {
    while (!game_over) {
        // Проверка таймера
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

        // Если все толстяки вышли — Кук больше не нужен
        if (alive_fat_men <= 0) {
            check_all_fat_men_dead();
            mtx.unlock();
            break;
        }

        // Кук ждет, пока толстяки поедят
        if (!cook_turn) {
            mtx.unlock();
            this_thread::yield();
            continue;
        }

        // Кук кладет еду
        dish1 += efficiency_factor;
        dish2 += efficiency_factor;
        dish3 += efficiency_factor;

        cook_turn = false;

        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(10)); // Задержка
    }
}

void run_test(string test_name, int glut, int eff) {
    reset_state();
    gluttony = glut;
    efficiency_factor = eff;

    cout << endl;
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