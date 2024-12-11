cmake_minimum_required(VERSION 3.0)
project(render)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(render 
    main.cpp 
    glad.h 
    glad.c 
)

target_link_libraries(render 
    glfw 
    OpenGL::GL
)