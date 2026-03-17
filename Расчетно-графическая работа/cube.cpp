#include <cmath>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <conio.h>
#include <random>
#include "cube.h"

namespace
{
    constexpr float PI = 3.1415926535f;

    float A = 0.0f;
    float B = 0.0f;
    float C = 0.0f;

    constexpr float cubeWidth = 44.0f;
    constexpr int width = 120;
    constexpr int height = 40;
    constexpr int screenSize = width * height;

    float zBuffer[screenSize];
    char screenBuffer[screenSize];
    WORD colorBuffer[screenSize];

    constexpr char backgroundAscii = ' ';
    constexpr float incrementSpeed = 1.8f;
    constexpr int distanceFromCam = 250;
    constexpr float K1 = 40.0f;

    std::mt19937 rng(123456u);
    std::uniform_int_distribution<int> colorDist(1, 15);

    HANDLE getConsoleHandle()
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void moveCursorToTop()
    {
        HANDLE hConsole = getConsoleHandle();
        COORD coord = { 0, 0 };
        SetConsoleCursorPosition(hConsole, coord);
    }

    void hideCursor()
    {
        HANDLE hConsole = getConsoleHandle();
        CONSOLE_CURSOR_INFO cursorInfo{};
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void showCursor()
    {
        HANDLE hConsole = getConsoleHandle();
        CONSOLE_CURSOR_INFO cursorInfo{};
        cursorInfo.dwSize = 20;
        cursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void resetConsoleColor()
    {
        SetConsoleTextAttribute(getConsoleHandle(), 7);
    }

    float calculateX(float i, float j, float k)
    {
        return j * sinf(A) * sinf(B) * cosf(C)
            - k * cosf(A) * sinf(B) * cosf(C)
            + j * cosf(A) * sinf(C)
            + k * sinf(A) * sinf(C)
            + i * cosf(B) * cosf(C);
    }

    float calculateY(float i, float j, float k)
    {
        return j * cosf(A) * cosf(C)
            + k * sinf(A) * cosf(C)
            - j * sinf(A) * sinf(B) * sinf(C)
            + k * cosf(A) * sinf(B) * sinf(C)
            - i * cosf(B) * sinf(C);
    }

    float calculateZ(float i, float j, float k)
    {
        return k * cosf(A) * cosf(B)
            - j * sinf(A) * cosf(B)
            + i * sinf(B);
    }

    WORD randomConsoleColor()
    {
        return static_cast<WORD>(colorDist(rng));
    }

    void clearBuffers()
    {
        memset(screenBuffer, backgroundAscii, sizeof(screenBuffer));
        memset(zBuffer, 0, sizeof(zBuffer));

        for (int i = 0; i < screenSize; ++i)
        {
            colorBuffer[i] = 7;
        }
    }

    void calculateSurface(float cubeX, float cubeY, float cubeZ, char ch)
    {
        const float x = calculateX(cubeX, cubeY, cubeZ);
        const float y = calculateY(cubeX, cubeY, cubeZ);
        const float z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

        if (z <= 0.0f)
        {
            return;
        }

        const float invZ = 1.0f / z;

        const int xp = static_cast<int>(width / 2 + K1 * invZ * x * 2.0f);
        const int yp = static_cast<int>(height / 2 + K1 * invZ * y);

        if (xp < 0 || xp >= width || yp < 0 || yp >= height)
        {
            return;
        }

        const int idx = xp + yp * width;

        if (invZ > zBuffer[idx])
        {
            zBuffer[idx] = invZ;
            screenBuffer[idx] = ch;
            colorBuffer[idx] = randomConsoleColor();
        }
    }

    void renderFrame()
    {
        clearBuffers();

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateSurface(cubeWidth, cubeY, cubeX, '+');
                calculateSurface(-cubeWidth, cubeY, -cubeX, '^');
                calculateSurface(-cubeX, cubeY, cubeWidth, '1');
            }
        }

        moveCursorToTop();

        HANDLE hConsole = getConsoleHandle();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const int idx = x + y * width;
                SetConsoleTextAttribute(hConsole, colorBuffer[idx]);
                putchar(screenBuffer[idx]);
            }
            putchar('\n');
        }

        resetConsoleColor();
    }

    void setupConsole()
    {
        HANDLE hConsole = getConsoleHandle();

        COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height + 1) };
        SetConsoleScreenBufferSize(hConsole, bufferSize);

        SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

        hideCursor();
        moveCursorToTop();
    }
}

void runCube()
{
    A = 0.0f;
    B = 0.0f;
    C = 0.0f;

    setupConsole();

    while (true)
    {
        if (_kbhit())
        {
            const int key = _getch();
            if (key == 27) // ESC
            {
                break;
            }
        }

        renderFrame();

        A += 0.005f;
        B += 0.005f;
        C += 0.002f;

        Sleep(1);
    }

    resetConsoleColor();
    showCursor();
    system("cls");
}