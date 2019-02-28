//
// Created by psaid on 28/02/19.
//

#include "../include/opencl_setup.h"

int create_program(cl_program *program, cl_context *context, cl_kernel *kernel, cl_command_queue * queue) {
    char *value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id *platforms;
    cl_uint deviceCount;
    cl_device_id *devices;
    cl_uint maxComputeUnits;


    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);


    clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
    devices = (cl_device_id *) malloc(sizeof(cl_device_id) * deviceCount);
    clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);


    clGetDeviceInfo(devices[0], CL_DEVICE_NAME, 0, NULL, &valueSize);
    value = (char *) malloc(valueSize);
    clGetDeviceInfo(devices[0], CL_DEVICE_NAME, valueSize, value, NULL);
    printf("Device: %s\n", value);
    free(value);

    *context = clCreateContext(NULL, deviceCount, devices, NULL, NULL, NULL);

    FILE *programHandle;
    size_t programSize, kernelSourceSize;
    char *programBuffer, *kernelSource;

    // get size of kernel source
    programHandle = fopen("../src/kernel.cl", "r");
    fseek(programHandle, 0, SEEK_END);
    programSize = (size_t) ftell(programHandle);
    rewind(programHandle);

    // read kernel source into buffer
    programBuffer = (char *) malloc(programSize + 1);
    programBuffer[programSize] = '\0';
    fread(programBuffer, sizeof(char), programSize, programHandle);
    fclose(programHandle);

    // create program from buffer
    cl_int err = 0;
    *program = clCreateProgramWithSource(*context, 1,
                                         (const char **) &programBuffer, &programSize, &err);
    if (err != 0) {
        printf("Error creating program\n");
        exit(1);
    }

    if (clBuildProgram(*program, 1, devices, NULL, NULL, NULL) != CL_SUCCESS) {
        printf("Error building program\n");
        exit(1);
    }


    *kernel = clCreateKernel(*program, "SAXPY", &err);
    if (err != 0) {
        printf("Error building kernel\n");
        exit(1);
    }

    *queue = clCreateCommandQueueWithProperties(*context, devices[0], NULL, &err);
    if (err != 0) {
        printf("Error creating Command Queue\n");
        exit(1);
    }
    free(programBuffer);

}