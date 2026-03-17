#include <GL/freeglut.h>
#include <vector>
#include <math.h>
float T_dna = 20.0f;     // Температура дна 
float T_vody = 20.0f;    // Температура воды
float koeff = 0.001f;     // Скорость нагрева (0.01 = медленно)
int kadr = 0;
int Rx = 1920;
int Ry = 1080;
struct Puzyrek {
    int x, y;
    int size; //Радиус
};
std::vector<Puzyrek> puzyrki;
void createPuzyrek() {
    Puzyrek Grisha;
    Grisha.x = rand() % Rx - Rx/2;
    Grisha.y = -Ry/2;
    Grisha.size = 1;
    puzyrki.push_back(Grisha);
}
void update(int value) {
    kadr++;
    T_vody += (T_dna - T_vody) * koeff;
    if (T_vody > 100) {
        T_vody = 100;
    }
    if (rand() % 100 < T_vody - 20) {  // при 20°C - 0%, при 100°C - 80%
        createPuzyrek();
    }

    // Двигаем пузырьки
    for (int i = 0; i < puzyrki.size(); i++) {
        puzyrki[i].y += 5;
        if(kadr%3==0){ puzyrki[i].size += 1; }
        // Удаляем улетевшие
        if (puzyrki[i].y > 540 + puzyrki[i].size) {
            puzyrki.erase(puzyrki.begin() + i);
            i--;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(5, update, 0);
}

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0f * 3.1415926f * (float)i / 50;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.5, 1.0); // Синий цвет
    //------------------------------------------------
    for (int i = 0; i < puzyrki.size(); i++) {
        drawCircle(puzyrki[i].x, puzyrki[i].y, puzyrki[i].size);

    }
    //------------------------------------------------
    glutSwapBuffers();
    //glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
    if (key == 27 or key == 13) exit(0);  // ESC or Enter

    if (key == '+') {  // Увеличить нагрев
        T_dna += 5.0f;
        if (T_dna > 200.0f) T_dna = 200.0f;
    }
    if (key == '-') {  // Уменьшить нагрев
        T_dna -= 5.0f;
        if (T_dna < 10.0f) T_dna = 10.0f;
    }

    printf("Dno: %.1f°C, Voda: %.1f°C\n", T_dna, T_vody); 

}



int main(int argc, char** argv) {
    

 
    glutInit(&argc, argv);
    //------------------------------------------------
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Rx, Ry);
    glutCreateWindow("Кипящий суп");

    //------------------------------------------------
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Rx/2, Rx/2, -Ry/2, Ry/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    //------------------------------------------------
    //------------------------------------------------

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //------------------------------------------------
    glutTimerFunc(0, update, 0);
    //------------------------------------------------
    glutMainLoop();
    return 0;
}
