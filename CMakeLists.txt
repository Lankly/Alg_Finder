cmake_minimum_required (VERSION 2.6)
project (Alg_Finder)
find_library(ncurses REQUIRED)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic -g")
add_executable(Alg_Finder
        main.c
	helpers.c
)
target_link_libraries(Alg_Finder ncurses m)
