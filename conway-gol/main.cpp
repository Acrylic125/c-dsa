#include <iostream>
#include <unistd.h> // for sleep()
#include <chrono>
#include <thread>
#include <ncurses.h>
#include <termios.h>

struct termios oldt, newt;

void set_raw_mode()
{
    tcgetattr(STDIN_FILENO, &oldt); // Get the current terminal attributes
    newt = oldt;
    cfmakeraw(&newt);                        // Set the terminal to raw mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply the new attributes
}

void reset_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old terminal attributes
}

char read_char()
{
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

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

const int GAME_STATE_SETUP = 0;
const int GAME_STATE_RUNNING = 1;

int gameState = GAME_STATE_SETUP;

const uint8_t GRID_X = 32;
const uint8_t GRID_Y = 16;

using Grid = int8_t[GRID_X][GRID_Y];
// using Screen = char[GRID_X][GRID_Y];
typedef char Screen[GRID_X][GRID_Y];

void redrawScreen(Screen &screen)
{
    clearScreen();
    for (int x = 0; x < GRID_X; x++)
    {
        std::string line = "";
        for (int y = 0; y < GRID_Y; y++)
        {
            line += screen[x][y];
        }
        std::cout << line << std::endl
                  << "\r";
    }
}

void setupGame(Grid &grid)
{
    Screen screen;
    for (auto &row : screen)
    {
        for (auto &cell : row)
        {
            cell = ' ';
        }
    }

    redrawScreen(screen);
    while (true)
    {
        char c = read_char();
        redrawScreen(screen);
        std::cout << "You pressed: " << c << std::endl
                  << "\r";

        if (c == 'q')
        {
            break;
        }
    }
}

int main()
{
    Grid grid;
    set_raw_mode();
    setupGame(grid);
    reset_terminal();
    return 0;
}