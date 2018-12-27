cmake_minimum_required(VERSION 3.12)
project(osLab)

set(CMAKE_CXX_STANDARD 17)

include_directories(header)

include_directories(libs)

#lib4aio:
add_subdirectory(libs/lib4aio)

add_executable(osLab
        main.cpp
        header/util/common.h
        source/util/common.cpp
        header/o_client.h
        source/o_client.cpp
        header/o_middle_server.h
        source/o_middle_server.cpp
        header/o_left_server.h
        header/o_right_server.h
        source/o_left_server.cpp
        source/o_right_server.cpp
        )

#link lib4aio:
target_link_libraries(osLab lib4aio)