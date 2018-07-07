cmake_minimum_required(VERSION 3.9)
project(BranchAnalyzer C)

set(CMAKE_C_STANDARD 99)

add_executable(BranchAnalyzer main.c predictors.c predictors.h predictorlib.c predictorlib.h linkedlist.c linkedlist.h)