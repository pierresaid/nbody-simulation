//
// Created by psaid on 28/02/19.
//

#ifndef NBODY_OPENCL_SETUP_H
#define NBODY_OPENCL_SETUP_H

#include <stdlib.h>
#include <CL/cl.h>
#include <stdio.h>
#include "config.h"
#include "body.h"
#include <GL/freeglut.h>

int create_program(cl_program *program, cl_context *context, cl_kernel *kernel, cl_command_queue *queue);
s_body *setup_bodies(s_config *config);

#endif //NBODY_OPENCL_SETUP_H
