#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <random>

float A = 0, B = 0, C = 0;

float cubeWidth = 44;
const int width = 120, height = 40;
float Zaxisbuffer[width * height];
char buffer[width * height];
int backgroundascoi = ' ';
int idx;

float incrementSpeed = 1.8f;
int distanceFromCam = 250;

float x, y, z;
float K1 = 40;
float kkk;
int xp, yp;

// Массивы для хранения цветов граней
WORD colorForChar[256]; // Таблица соответствия символов цветам

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

// Функция для генерации случайных цветов
void generateRandomColors()
{
    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(1, 15); // От 1 до 15 (исключаем 0 - черный)
    
    // Массив символов, которые используются для граней
    char faceChars[] = {'.', '+', '^', '1'};
    
    // Присваиваем случайный цвет каждому символу
    for (char ch : faceChars)
    {
        colorForChar[(unsigned char)ch] = colorDist(gen);
    }
}

// Функция для установки цвета консоли
void setConsoleColor(HANDLE hConsole, char ch)
{
    SetConsoleTextAttribute(hConsole, colorForChar[(unsigned char)ch]);
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

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Генерируем случайные цвета при запуске
    generateRandomColors();

    // Размер буфера консоли
    COORD bufferSize = { (SHORT)width, (SHORT)height + 1 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)height };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    hideCursor();
    
    // Очищаем экран перед началом анимации
    system("cls");

    while (true)
    {
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

        // Вывод буфера с цветами
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
                    putchar(buffer[k]);
                }
                else
                {
                    putchar(buffer[k]);
                }
            }
        }

        A += 0.005f;
        B += 0.005f;

        Sleep(1); // 1 миллисекунда
    }

    return 0;
}
