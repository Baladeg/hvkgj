#include <cmath>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <conio.h>
#include "cube.h"

float A = 0.0f, B = 0.0f, C = 0.0f;

float cubeWidth = 44.0f;
const int width = 120;
const int height = 40;

float Zaxisbuffer[width * height];
char buffer[width * height];
int backgroundascoi = ' ';
int idx;

float incrementSpeed = 1.8f;
int distanceFromCam = 250;

float x, y, z;
float K1 = 40.0f;
float kkk;
int xp, yp;

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

float calculateX(float i, float j, float k)
{
    return j * sin(A) * sin(B) * cos(C)
        - k * cos(A) * sin(B) * cos(C)
        + j * cos(A) * sin(C)
        + k * sin(A) * sin(C)
        + i * cos(B) * cos(C);
}

float calculateY(float i, float j, float k)
{
    return j * cos(A) * cos(C)
        + k * sin(A) * cos(C)
        - j * sin(A) * sin(B) * sin(C)
        + k * cos(A) * sin(B) * sin(C)
        - i * cos(B) * sin(C);
}

float calculateZ(float i, float j, float k)
{
    return k * cos(A) * cos(B)
        - j * sin(A) * cos(B)
        + i * sin(B);
}

void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch)
{
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    kkk = 1.0f / z;

    xp = (int)(width / 2 + K1 * kkk * x * 2);
    yp = (int)(height / 2 + K1 * kkk * y);

    idx = xp + yp * width;

    if (idx >= 0 && idx < width * height)
    {
        if (kkk > Zaxisbuffer[idx])
        {
            Zaxisbuffer[idx] = kkk;
            buffer[idx] = (char)ch;
        }
    }
}

void runCube()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = { (SHORT)width, (SHORT)(height + 1) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)height };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    hideCursor();
    moveCursorToTop();

    while (true)
    {
        if (_kbhit())
        {
            int key = _getch();
            if (key == 27) // ESC
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
            putchar(k % width ? buffer[k] : '\n');
        }

        A += 0.005f;
        B += 0.005f;

        Sleep(1);
    }

    system("cls");
}