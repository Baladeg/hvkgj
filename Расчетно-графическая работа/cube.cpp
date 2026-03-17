#include <cmath>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <random>
#include <conio.h>
#include "cube.h"

namespace
{
    float A = 0.0f;
    float B = 0.0f;
    float C = 0.0f;

    constexpr float cubeWidth = 44.0f;
    constexpr int width = 120;
    constexpr int height = 40;

    float Zaxisbuffer[width * height];
    char buffer[width * height];
    int backgroundascoi = ' ';
    int idx = 0;

    float incrementSpeed = 1.8f;
    int distanceFromCam = 250;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float K1 = 40.0f;
    float kkk = 0.0f;
    int xp = 0;
    int yp = 0;

    WORD colorForChar[256]{};

    void moveCursorToTop()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = { 0, 0 };
        SetConsoleCursorPosition(hConsole, coord);
    }

    void hideCursor()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void showCursor()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 20;
        cursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    std::mt19937 gen;

    void generateRandomColors()
    {
        std::random_device rd;
        gen.seed(rd());  // каждый запуск новый seed

        std::uniform_int_distribution<int> colorDist(1, 15);

        char faceChars[] = { '.', '+', '^', '1' };

        for (char ch : faceChars)
        {
            colorForChar[(unsigned char)ch] = colorDist(gen);
        }
    }

    void setConsoleColor(HANDLE hConsole, char ch)
    {
        WORD color = colorForChar[static_cast<unsigned char>(ch)];
        if (color == 0)
        {
            color = 7;
        }
        SetConsoleTextAttribute(hConsole, color);
    }

    void resetConsoleColor(HANDLE hConsole)
    {
        SetConsoleTextAttribute(hConsole, 7);
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

    void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch)
    {
        x = calculateX(cubeX, cubeY, cubeZ);
        y = calculateY(cubeX, cubeY, cubeZ);
        z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

        kkk = 1.0f / z;

        xp = static_cast<int>(width / 2 + K1 * kkk * x * 2);
        yp = static_cast<int>(height / 2 + K1 * kkk * y);

        idx = xp + yp * width;

        if (idx >= 0 && idx < width * height)
        {
            if (kkk > Zaxisbuffer[idx])
            {
                Zaxisbuffer[idx] = kkk;
                buffer[idx] = static_cast<char>(ch);
            }
        }
    }
}

void runCube()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    A = 0.0f;
    B = 0.0f;
    C = 0.0f;

    generateRandomColors();

    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height + 1) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    hideCursor();
    system("cls");

    while (true)
    {
        if (_kbhit())
        {
            int key = _getch();
            if (key == 27)
            {
                break;
            }
        }

        memset(buffer, backgroundascoi, sizeof(buffer));
        memset(Zaxisbuffer, 0, sizeof(Zaxisbuffer));

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

        for (int k = 0; k < width * height; k++)
        {
            if (k % width == 0 && k != 0)
            {
                putchar('\n');
            }
            else
            {
                if (buffer[k] != backgroundascoi)
                {
                    setConsoleColor(hConsole, buffer[k]);
                }
                else
                {
                    resetConsoleColor(hConsole);
                }

                putchar(buffer[k]);
            }
        }

        A += 0.005f;
        B += 0.005f;

        Sleep(1);
    }

    resetConsoleColor(hConsole);
    showCursor();
    system("cls");
}
