#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include "boiling_soup.h"

namespace
{
    constexpr int Rx = 1280;
    constexpr int Ry = 720;

    float T_dna = 20.0f;
    float T_vody = 20.0f;
    float koeff = 0.001f;
    int kadr = 0;

    struct Bubble
    {
        float x;
        float y;
        float size;
        float r;
        float g;
        float b;
        float speedY;
        float growthRate;
    };

    std::vector<Bubble> bubbles;

    std::mt19937 rng(987654u);
    std::uniform_real_distribution<float> xDist(-Rx / 2.0f, Rx / 2.0f);
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);
    std::uniform_real_distribution<float> speedDist(2.0f, 6.0f);
    std::uniform_real_distribution<float> growthDist(0.1f, 0.5f);
    std::uniform_int_distribution<int> chanceDist(0, 99);

    void createBubble()
    {
        Bubble bubble{};
        bubble.x = xDist(rng);
        bubble.y = -Ry / 2.0f;
        bubble.size = 2.0f;
        bubble.r = colorDist(rng);
        bubble.g = colorDist(rng);
        bubble.b = colorDist(rng);
        bubble.speedY = speedDist(rng);
        bubble.growthRate = growthDist(rng);

        bubbles.push_back(bubble);
    }

    void drawCircle(float x, float y, float radius)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for (int i = 0; i <= 50; ++i)
        {
            const float angle = 2.0f * 3.1415926f * static_cast<float>(i) / 50.0f;
            const float dx = radius * cosf(angle);
            const float dy = radius * sinf(angle);
            glVertex2f(x + dx, y + dy);
        }

        glEnd();
    }

    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        for (const Bubble& bubble : bubbles)
        {
            glColor3f(bubble.r, bubble.g, bubble.b);
            drawCircle(bubble.x, bubble.y, bubble.size);
        }

        glutSwapBuffers();
    }

    void update(int)
    {
        ++kadr;

        T_vody += (T_dna - T_vody) * koeff;
        if (T_vody > 100.0f)
        {
            T_vody = 100.0f;
        }

        const int spawnChance = static_cast<int>(T_vody - 20.0f);
        if (spawnChance > 0 && chanceDist(rng) < spawnChance)
        {
            createBubble();
        }

        for (int i = 0; i < static_cast<int>(bubbles.size()); ++i)
        {
            bubbles[i].y += bubbles[i].speedY;

            if (kadr % 3 == 0)
            {
                bubbles[i].size += bubbles[i].growthRate;
            }

            if (bubbles[i].y > Ry / 2.0f + bubbles[i].size)
            {
                bubbles.erase(bubbles.begin() + i);
                --i;
            }
        }

        glutPostRedisplay();
        glutTimerFunc(5, update, 0);
    }

    void keyboard(unsigned char key, int, int)
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
        else if (key == '-')
        {
            T_dna -= 5.0f;
            if (T_dna < 10.0f)
            {
                T_dna = 10.0f;
            }
        }

        printf("Dno: %.1f C, Voda: %.1f C\n", T_dna, T_vody);
    }

    void initScene()
    {
        bubbles.clear();
        T_dna = 20.0f;
        T_vody = 20.0f;
        kadr = 0;
    }
}

void runBoilingSoup(int argc, char** argv)
{
    initScene();

    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Rx, Ry);
    glutCreateWindow("Boiling soup");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Rx / 2.0, Rx / 2.0, -Ry / 2.0, Ry / 2.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
}