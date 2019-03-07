#include <math.h>
#include "../include/setup.h"
#include "../include/data.h"
//#include <GL/glut.h>
//#include <GL/freeglut.h>

size_t globalWorkSize[] = {1, 0, 0};
cl_command_queue queue;
cl_kernel kernel;
s_data data;
cl_mem bodies_buffer;
int paused = 0;

void process();

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glPointSize(1);
    glBegin(GL_POINTS);

    int i = 0;
    while (i < data.config.nb_bodies) {
        glVertex2i(data.bodies[i].x, data.bodies[i].y);
        ++i;
    }


    glEnd();
    glFlush();
}

const double s_soft = 1000000;
const float gravity = 0.66742;

void process() {
    if (paused == 0) {

//        int i = 0;
//        while (i < data.config.nb_bodies) {
//            data.bodies[i].x += data.bodies[i].speed_x;
//            data.bodies[i].y += data.bodies[i].speed_y;
//            ++i;
//        }

        clEnqueueWriteBuffer(queue, bodies_buffer, CL_TRUE, 0, sizeof(s_body) * data.config.nb_bodies, data.bodies, 0,
                             NULL,
                             NULL);


        cl_int err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bodies_buffer);
        err = clSetKernelArg(kernel, 1, sizeof(int), &data.config.nb_bodies);
        if (err != CL_SUCCESS) {
            printf("Set Arg error : %d\n", err);
            exit(0);
        }

        clEnqueueNDRangeKernel(queue, kernel,
                               1,
                               NULL,
                               globalWorkSize,
                               NULL,
                               0, NULL, NULL);
        clFlush(queue);
        clFinish(queue);
        clEnqueueReadBuffer(queue, bodies_buffer, CL_TRUE, 0, sizeof(s_body) * data.config.nb_bodies, data.bodies, 0,
                            NULL,
                            NULL);

        glutPostRedisplay();
    }

    glutTimerFunc(1000 / 60, process, 0);
}

void keyboardCB(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutLeaveMainLoop();
        case GLUT_KEY_F2:
            paused = paused == 0 ? 1 : 0;
            return;
    }
    glutPostRedisplay();
}


void setup_opengl(s_config *config) {
    int argc = 1;
    char *argv[1] = {(char *) "Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("nbody-simulator");
    glutSpecialFunc(keyboardCB);
    glClearColor(0, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, config->map_size, 0.0, config->map_size);
}

int main(int argc, char **argv) {
    cl_program program;
    cl_context context;

    data.config.nb_bodies = 7000;
    globalWorkSize[0] = (size_t) data.config.nb_bodies;
    data.config.map_size = 100000;
    create_program(&program, &context, &kernel, &queue);
    setup_opengl(&data.config);
    data.bodies = setup_bodies(&data.config);

    data.bodies[0].x = data.config.map_size - (4 * (data.config.map_size / 10));
    data.bodies[0].y = data.config.map_size - (4 * (data.config.map_size / 10));
    data.bodies[0].mass = 1300000000;
    data.bodies[0].speed_x = 0;
    data.bodies[0].speed_y = 0;

    bodies_buffer = clCreateBuffer(context,
                                   CL_MEM_READ_WRITE,
                                   sizeof(s_body) * (data.config.nb_bodies),
                                   data.bodies, NULL);

    glutDisplayFunc(display);
    process();
    glutMainLoop();

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

