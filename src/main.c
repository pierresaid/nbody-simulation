#include "../include/opencl_setup.h"
//#include <GL/glut.h>
#include <GL/freeglut.h>

const size_t globalWorkSize[] = {1, 0, 0};
cl_command_queue queue;
cl_kernel kernel;

void process();

void display() {
    glColor3f(1, 1, 1);
    glPointSize(1);
    glBegin(GL_POINTS);
    int x, y;
    x = rand() % 640;
    y = rand() % 480;

    glVertex2i(x, y);
    glEnd();

    glFlush();
}

void loop(cl_command_queue queue, cl_kernel kernel) {
    glutIdleFunc(process);
    glutMainLoop();
}

void process() {
    clEnqueueNDRangeKernel(queue, kernel,
                           1,
                           NULL,
                           globalWorkSize,
                           NULL,
                           0, NULL, NULL);
    clFlush(queue);
    clFinish(queue);
    display();
}

void keyboardCB(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutLeaveMainLoop();
            return;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    cl_program program;
    cl_context context;
    create_program(&program, &context, &kernel, &queue);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Randomly generated points");
    glutSpecialFunc(keyboardCB);
    glClearColor(0, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);

    loop(queue, kernel);


    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}
