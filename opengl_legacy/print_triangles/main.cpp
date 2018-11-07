#include <GL/freeglut.h>

void Display();
void Initialize();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Hello World");
    glutDisplayFunc(Display);
    Initialize();
    glutMainLoop();

    return 0;
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-5.0, -5.0);
        glVertex2f(5.0, -5.0);
        glVertex2f(0.0, 5.0);
    glEnd();

    glFlush();
}

void Initialize()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
}
