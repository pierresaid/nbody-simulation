cmake_minimum_required(VERSION 3.10)
project(nbody C)

find_package (OpenCL REQUIRED)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)

set(CMAKE_C_STANDARD 99)

set(HEADERS )

set(SOURCES src/opencl_setup.c include/opencl_setup.h)

#add_compile_definitions(OPENCL_HOST=true)

#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O3 -Wall -Wextra -Wno-deprecated")

add_executable(nbody src/main.c ${HEADERS} ${SOURCES})

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#find_package(JPEG REQUIRED)
#find_package(GLEW REQUIRED)

#find_package (GLM REQUIRED)

target_link_libraries(nbody ${GLUT_LIBRARIES} ${OpenCL_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(nbody PRIVATE  ${GLUT_INCLUDE_DIR} ${OPENCL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
