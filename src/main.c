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


//    int i = 0;
//    int j;
//    while (i < data.config.nb_bodies) {
//        j = 0;
//        while (j < data.config.nb_bodies) {
////            if (!(data.bodies[i]->x == data.bodies[j]->x && data.bodies[i]->y == data.bodies[j]->y)) {
//
//            float dx = data.bodies[i].x - data.bodies[j].x;
//            float dy = data.bodies[i].y - data.bodies[j].y;
//            double r = sqrt((dx) * (dx) +
//                            (dy) * (dy) + s_soft);
//            if (r > 0) {
//                double k = gravity * data.bodies[j].mass / (r * r * r);
//
////                    printf("%f\n", r);
//                data.bodies[i].speed_x += k * (data.bodies[j].x - data.bodies[i].x);
//                data.bodies[i].speed_y += k * (data.bodies[j].y - data.bodies[i].y);
//            } else {
////                force.x = force.y = 0;
//            }
////            }
//
//            ++j;
//        }
//        ++i;
//
//    }

    int i = 0;
//    i = 0;
    while (i < data.config.nb_bodies) {
        data.bodies[i].x += data.bodies[i].speed_x;
        data.bodies[i].y += data.bodies[i].speed_y;
        ++i;
    }

//    CheckError (error);

//printf("slaope : %u\n", sizeof(s_body) * data.config.nb_bodies);
    clEnqueueWriteBuffer(queue, bodies_buffer, CL_TRUE, 0, sizeof(s_body) * data.config.nb_bodies, data.bodies, 0, NULL,
                         NULL);


    cl_int err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bodies_buffer);
//    cl_int err = clSetKernelArg(kernel, 0, sizeof(s_body) * data.config.nb_bodies, bodies_buffer);
    if (err != CL_SUCCESS)
    {
//        CL_INVALID_ARG_INDEX
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
    clEnqueueReadBuffer(queue, bodies_buffer, CL_TRUE, 0, sizeof(s_body) * data.config.nb_bodies, data.bodies, 0, NULL,
                        NULL);

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, process, 0);
}

int main(int argc, char **argv) {
    cl_program program;
    cl_context context;

    data.config.nb_bodies = 10000;
    globalWorkSize[0] = (size_t) data.config.nb_bodies;
    data.config.map_size = 100000;
    create_program(&program, &context, &kernel, &queue);
    setup_opengl(&data.config);
    data.bodies = setup_bodies(&data.config);

    data.bodies[0].x = 2 * (data.config.map_size / 10);
    data.bodies[0].y = 2 * (data.config.map_size / 10);
    data.bodies[0].mass = 100000000;

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

