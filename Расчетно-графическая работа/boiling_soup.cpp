#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "boiling_soup.h"

float T_dna = 20.0f;
float T_vody = 20.0f;
float koeff = 0.001f;
int kadr = 0;
int Rx = 1280;
int Ry = 720;

struct Puzyrek
{
    int x, y;
    int size;
};

std::vector<Puzyrek> puzyrki;

void createPuzyrek()
{
    Puzyrek p;
    p.x = rand() % Rx - Rx / 2;
    p.y = -Ry / 2;
    p.size = 1;
    puzyrki.push_back(p);
}

void update(int value)
{
    kadr++;
    T_vody += (T_dna - T_vody) * koeff;

    if (T_vody > 100.0f)
    {
        T_vody = 100.0f;
    }

    if (rand() % 100 < (int)(T_vody - 20.0f))
    {
        createPuzyrek();
    }

    for (int i = 0; i < (int)puzyrki.size(); i++)
    {
        puzyrki[i].y += 5;

        if (kadr % 3 == 0)
        {
            puzyrki[i].size += 1;
        }

        if (puzyrki[i].y > Ry / 2 + puzyrki[i].size)
        {
            puzyrki.erase(puzyrki.begin() + i);
            i--;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(5, update, 0);
}

void drawCircle(float x, float y, float radius)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (int i = 0; i <= 50; i++)
    {
        float angle = 2.0f * 3.1415926f * (float)i / 50.0f;
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.0f, 0.5f, 1.0f);

    for (int i = 0; i < (int)puzyrki.size(); i++)
    {
        drawCircle((float)puzyrki[i].x, (float)puzyrki[i].y, (float)puzyrki[i].size);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27 || key == 13) // ESC или Enter
    {
        glutLeaveMainLoop();
        return;
    }

    if (key == '+')
    {
        T_dna += 5.0f;
        if (T_dna > 200.0f)
        {
            T_dna = 200.0f;
        }
    }

    if (key == '-')
    {
        T_dna -= 5.0f;
        if (T_dna < 10.0f)
        {
            T_dna = 10.0f;
        }
    }

    printf("Dno: %.1f C, Voda: %.1f C\n", T_dna, T_vody);
}

void runBoilingSoup(int argc, char** argv)
{
    puzyrki.clear();
    T_dna = 20.0f;
    T_vody = 20.0f;
    kadr = 0;

    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Rx, Ry);
    glutCreateWindow("Boiling soup");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Rx / 2, Rx / 2, -Ry / 2, Ry / 2, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
}