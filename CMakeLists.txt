cmake_minimum_required(VERSION 3.27)

project(quad LANGUAGES C)

# OpenCL
add_compile_definitions(CL_TARGET_OPENCL_VERSION=300)
find_package(OpenCL REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# X11
find_package(X11 REQUIRED)

add_executable(${PROJECT_NAME} quad.c)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL OpenGL::OpenGL OpenGL::GLX OpenGL::GLU X11::X11)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
