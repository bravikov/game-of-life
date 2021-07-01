#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std::chrono_literals;

using Field = std::vector<std::vector<bool>>;

const int w = 60;
const int h = 30;

void clearScreen()
{
    for (int y = 0; y < h; y++) {
        std::cout << std::endl;
    }
}

void print(const Field& field)
{
    clearScreen();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            std::cout << (field[x][y] ? "▣" : "▢");
        }
        std::cout << std::endl;
    }
}

void next(Field& field);

int main()
{
    Field field(w, std::vector<bool>(h, false));

    // Blinker
    /*field[1][0] = true;
    field[1][1] = true;
    field[1][2] = true;*/

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    // give "true" 1/4 of the time
    // give "false" 3/4 of the time
    std::bernoulli_distribution distribution(0.3);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            field[x][y] = distribution(randomGenerator);
        }
    }

    while (true) {
        print(field);
        std::this_thread::sleep_for(100ms);
        next(field);
    }

    return 0;
}

bool state(const Field& field, const int x, const int y)
{
    if (x < 0 || x >= w) {
        return false;
    }
    if (y < 0 || y >= h) {
        return false;
    }
    return field[x][y];
}

void next(Field& field)
{
    Field newField(w, std::vector<bool>(h, false));

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    if (state(field, x + dx, y + dy)) {
                        count++;
                    }
                }
            }
            if (count == 3) {
                newField[x][y] = true;
            }
            if (count == 2 && field[x][y] == true) {
                newField[x][y] = true;
            }
        }
    }
    field = newField;
}
