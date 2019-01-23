
cmake_minimum_required(VERSION 3.0)
project(project C)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCE "src/*.c" "src/*.h")
add_executable(project "${SOURCE}" src/LIBs/Audio.c src/LIBs/Audio.h)

include_directories("/usr/include/SDL2")
target_link_libraries(project m SDL2 SDL2_gfx)