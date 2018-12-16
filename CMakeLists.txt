cmake_minimum_required(VERSION 3.12)
project(osLab)

set(CMAKE_CXX_STANDARD 17)

add_executable(osLab main.cpp header/component_reader.h header/component_archiver.h header/component_separator.h header/component_programmer.h)