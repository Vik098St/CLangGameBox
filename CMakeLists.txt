cmake_minimum_required(VERSION 3.16)
project(Tetris C)

set(CMAKE_C_STANDARD 99)

add_executable(Tetris main.c tetris.h snake.c snake.h drawable.c drawable.h arkanoid.c arkanoid.h tetris.c)