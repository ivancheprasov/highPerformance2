cmake_minimum_required(VERSION 3.19)
project(highPerformance2 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(highPerformance2 main.c file.c file.h const.h high_life.c high_life.h utils.c utils.h mpi.c mpi.h)
target_link_libraries(highPerformance2 ${MPI_C_LIBRARIES})
target_link_libraries(highPerformance2 m)