#include <iostream>
#include <unistd.h> // for sleep()
#include <chrono>
#include <thread>

void clearScreen()
{
    std::cout << "\033[2J"; // Clear the entire screen
    std::cout << "\033[H";  // Move cursor to the home position (top-left corner)
}

void moveCursor(int row, int col)
{
    std::cout << "\033[" << row << ";" << col << "H"; // Move cursor to (row, col)
}

void showDrafts()
{
    clearScreen();
    std::cout << "Draft 1: This is the first draft.\n";
    std::cout << "Draft 2: This is the second draft.\n";
    std::cout << "Draft 3: This is the third draft.\n";
    std::cout << "Draft 4: This is the fourth draft.\n";
}

void updateDraft(int line, const std::string &newText)
{
    moveCursor(line, 1);     // Move cursor to the start of the specified line
    std::cout << "\33[2K\r"; // Clear the line
    std::cout << newText;    // Output the new text
    std::cout << std::endl;  // Move to the next line
}

int main()
{
    showDrafts();
    int count = 0;
    while (true)
    {
        // clearScreen();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        updateDraft(count % 4 + 1, "Hello " + std::to_string(count));
        count++;
    }

    return 0;
}

// #include <iostream>

// using namespace std;

// const uint8_t GRID_X = 64;
// const uint8_t GRID_Y = 64;

// const uint8_t DEAD = 0;
// const uint8_t ALIVE = 1;

// int main()
// {
//     uint8_t grid[GRID_X][GRID_Y];
//     // for (int x = 0; x < GRID_X; x++)
//     // {
//     // }
//     std::cout << "Test: " << std::begin(grid) << std::endl;
//     std::cout << "Hello 1";
//     std::cout << "\r\rHello 2";
//     return 0;
// }