cmake_minimum_required(VERSION 3.0.0)
project(ex1 VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(ex1 main.c)

# Set variables holding flags for gcc 
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror") 
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDBUG")

# Set the flags for gcc (can also be done using target_compile_options and a couple of other ways) 
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
