#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <exception>

using namespace std::chrono_literals;

struct Field
{
    Field(): cells(w, std::vector<bool>(h, false))
    {}

    int w = 30;
    int h = 30;

    std::vector<std::vector<bool>> cells;

    Field& operator=(const Field& other)
    {
        this->w = other.w;
        this->h = other.h;
        this->cells = other.cells;
        return *this;
    }
};



void clearScreen(const int h)
{
    for (int y = 0; y < h; y++) {
        std::cout << std::endl;
    }
}

void print(const Field& field)
{
    clearScreen(field.h);
    for (int y = 0; y < field.h; y++) {
        for (int x = 0; x < field.w; x++) {
            std::cout << (field.cells[x][y] ? "▣ " : "▢ ");
        }
        std::cout << std::endl;
    }
}

void next(Field& field);
void fixCyclicPoint(int& value, const int size);

/**
 * Creates Glider in [x, y]
 *
 * ▣ ▢ ▢
 * ▢ ▣ ▣
 * ▣ ▣ ▢
 */
void addGlider(Field& field, const int x, const int y)
{
    for (int y2 = y; y2 < y + 3; y2++) {
        for (int x2 = x; x2 < x + 3; x2++) {
            field.cells[x2][y2] = false;
        }
    }

    field.cells[x+0][y+0] = true;

    field.cells[x+1][y+1] = true;
    field.cells[x+2][y+1] = true;

    field.cells[x+0][y+2] = true;
    field.cells[x+1][y+2] = true;
}


/**
 * Creates Blinker in [x, y]
 *
 * ▢ ▣ ▢
 * ▢ ▣ ▢
 * ▢ ▣ ▢
 */
void addBlinker(Field& field, const int x, const int y)
{
    for (int y2 = y; y2 < y + 3; y2++) {
        for (int x2 = x; x2 < x + 3; x2++) {
            field.cells[x2][y2] = false;
        }
    }

    field.cells[x+1][y+0] = true;
    field.cells[x+1][y+1] = true;
    field.cells[x+1][y+2] = true;
}

int main()
{
    Field field;

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::bernoulli_distribution distribution(0.25);

    for (int y = 0; y < field.h; y++) {
        for (int x = 0; x < field.w; x++) {
            field.cells[x][y] = distribution(randomGenerator);
        }
    }

    addGlider(field, 0, 0);
    addBlinker(field, 5, 5);
    addBlinker(field, 10, 10);
    addGlider(field, 20, 20);
    addGlider(field, 15, 15);
    addGlider(field, 10, 15);

    while (true) {
        print(field);
        std::this_thread::sleep_for(500ms);
        next(field);
    }

    return 0;
}

/**
 * Returns the cell state.
 *
 * Assumes that field has borders where there is no life
 */
bool state(const Field& field, const int x, const int y)
{
    if (x < 0 || x >= field.w) {
        return false;
    }
    if (y < 0 || y >= field.h) {
        return false;
    }
    return field.cells[x][y];
}

/**
 * Returns the cell state.
 *
 * Assumes that field is cyclic.
 */
bool stateOnCyclicField(const Field& field, int x, int y)
{
    fixCyclicPoint(x, field.w);
    fixCyclicPoint(y, field.h);

    return field.cells[x][y];
}

void fixCyclicPoint(int& value, const int size)
{
    if (value < 0) {
        value = size + (value % size);
    }
    if (value >= size) {
        value %= size;
    }
}

void next(Field& field)
{
    Field newField;

    for (int y = 0; y < field.h; y++) {
        for (int x = 0; x < field.w; x++) {
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    if (stateOnCyclicField(field, x + dx, y + dy)) {
                        count++;
                    }
                }
            }
            if (count == 3) {
                newField.cells[x][y] = true;
            }
            else if (count == 2 && field.cells[x][y] == true) {
                newField.cells[x][y] = true;
            }
        }
    }
    field = newField;
}
