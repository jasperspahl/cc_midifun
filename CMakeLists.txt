cmake_minimum_required(VERSION 3.24)
project(midifun C)

set(CMAKE_C_STANDARD 17)

link_libraries(rtmidi pthread m c)
add_executable(midifun main.c types.h midi_callback.c midi_callback.h)
