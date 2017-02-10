cmake_minimum_required (VERSION 2.6)
project (Cube_Sim)
find_library(ncurses REQUIRED)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic -g")
add_executable(Cube_Sim
        main.c
	helpers.c
        state.c
)
target_link_libraries(Cube_Sim ncurses m)
