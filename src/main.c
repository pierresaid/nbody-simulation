#include "../include/opencl_setup.h"

int main() {
    cl_program program;
    cl_context context;
    cl_kernel kernel;
    cl_command_queue queue;
    create_program(&program, &context, &kernel, &queue);

    const size_t globalWorkSize [] = { 1, 0, 0 };

    clEnqueueNDRangeKernel(queue, kernel,
                           1,
                           NULL,
                           globalWorkSize,
                           NULL,
                           0, NULL, NULL);
    clFlush(queue);
    clFinish(queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}
