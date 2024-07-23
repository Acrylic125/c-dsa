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

const uint8_t GRID_X = 16;
const uint8_t GRID_Y = 16;

using Grid = int8_t[GRID_X][GRID_Y];
typedef char Screen[GRID_X][GRID_Y];

typedef uint8_t Pos[2];

void redrawScreen(Screen &screen, Pos &cursor)
{
    // const int screenWidth = sizeof(screen) / sizeof(screen[0]);
    // const int screenHeight = sizeof(screen[0]) / sizeof(screen[0][0]);
    const int screenWidth = GRID_X;
    const int screenHeight = GRID_Y;
    clearScreen();
    for (int y = 0; y < screenHeight; y++)
    {
        std::string line = "";
        for (int x = 0; x < screenWidth; x++)
        {
            if (x == cursor[1] && y == cursor[0])
            {
                char v = screen[x][y];
                std::string a = std::string(1, v);
                std::string s = "\033[33m" + a + "\033[0m";
                line += s;
                // line += "A";
            }
            else
            {
                line += screen[x][y];
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
            screen[y][x] = ' ';
        }
    }

    screen[2][0] = '@';
    Pos cursor = {0, 0};
    redrawScreen(screen, cursor);

    int cursorX, cursorY;

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
            cursorX--;
            break;
        case 'a':
            cursorY--;
            break;
        case 's':
            cursorX++;
            break;
        case 'd':
            cursorY++;
            break;
        default:
            break;
        }

        // std::cout << "You pressed: " << c << std::endl
        //           << "\r";

        // std::cout << "Before Cursor: " << cursorX << ", " << cursorY << std::endl
        //           << "\r";

        // cursorX = cursorX % screenWidth;
        // cursorY = cursorY % screenHeight;
        cursorX = positive_modulo(cursorX, screenWidth);
        cursorY = positive_modulo(cursorY, screenHeight);
        screen[cursorY][cursorX] = '@';

        Pos cursor = {cursorX, cursorY};
        redrawScreen(screen, cursor);
        std::cout << "Cursor: " << cursorX << ", " << cursorY << std::endl
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