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

const uint8_t GRID_X = 64;
const uint8_t GRID_Y = 16;

using Grid = int8_t[GRID_Y][GRID_X];
typedef char Screen[GRID_Y][GRID_X];

typedef uint8_t Pos[2];

void redrawScreen(Screen &screen, Pos &cursor)
{
    const int screenHeight = sizeof(screen) / sizeof(screen[0]);
    const int screenWidth = sizeof(screen[0]) / sizeof(screen[0][0]);
    clearScreen();
    for (int y = 0; y < screenHeight; y++)
    {
        std::string line = "";
        for (int x = 0; x < screenWidth; x++)
        {
            if (x == cursor[0] && y == cursor[1])
            {
                char v = screen[y][x];
                std::string a = std::string(1, v);
                std::string s = "\033[33m" + a + "\033[0m";
                line += s;
            }
            else
            {
                line += screen[y][x];
            }
        }
        std::cout << line << std::endl
                  << "\r";
    }
}

int positive_modulo(int i, int n)
{
    return (i % n + n) % n;
}

void setupGame(Grid &grid)
{
    Screen screen;

    const int screenHeight = sizeof(screen) / sizeof(screen[0]);
    const int screenWidth = sizeof(screen[0]) / sizeof(screen[0][0]);

    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            screen[y][x] = '.';
        }
    }

    Pos cursor = {0, 0};
    redrawScreen(screen, cursor);

    while (true)
    {
        char c = read_char();

        if (c == 'q')
        {
            break;
        }
        switch (c)
        {
        case 'w':
            cursor[1]--;
            break;
        case 'a':
            cursor[0]--;
            break;
        case 's':
            cursor[1]++;
            break;
        case 'd':
            cursor[0]++;
            break;
        default:
            break;
        }

        cursor[0] = positive_modulo(cursor[0], screenWidth);
        cursor[1] = positive_modulo(cursor[1], screenHeight);
        screen[cursor[1]][cursor[0]] = '@';

        redrawScreen(screen, cursor);
        std::cout << "Cursor: " << cursor[0] << ", " << cursor[1] << std::endl
                  << "\r"
                  << "Screen: " << screenWidth << ", " << screenHeight << std::endl
                  << "\r";
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