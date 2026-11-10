#include <iostream>
#include <thread>
#include <mutex>
#include <string>

using namespace std;

int coins = 101;
int initial_coins = 0;
int Bob_coins = 0;
int Tom_coins = 0;

mutex mtx;

void coin_sharing(string name, int& thief_coins, int& companion_coins) {
    while (true) {
        mtx.lock();

        if (coins <= 0) {
            mtx.unlock();
            break;
        }

        if (thief_coins <= companion_coins) {
            if (coins == 1 && (initial_coins % 2 != 0)) {
                mtx.unlock();
                break;
            }

            thief_coins++;
            coins--;

            cout << name << " :" << thief_coins << " " << companion_coins
                << " (Осталось в мешке: " << coins << ")" << endl;
        }

        mtx.unlock();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    initial_coins = coins;

    cout << "Начало дележа. Всего монет: " << coins << endl;

    thread bob(coin_sharing, "Bob", ref(Bob_coins), ref(Tom_coins));
    thread tom(coin_sharing, "Tom", ref(Tom_coins), ref(Bob_coins));

    bob.join();
    tom.join();

    cout << "\nИтоги" << endl;
    cout << "Боб: " << Bob_coins << endl;
    cout << "Том: " << Tom_coins << endl;
    cout << "Осталось в мешке (покойнику): " << coins << endl;
    cout << "Сумма монет: " << Bob_coins + Tom_coins + coins << endl;

    system("pause");
    return 0;
}