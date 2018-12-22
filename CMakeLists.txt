cmake_minimum_required(VERSION 3.12)
project(osLab)

set(CMAKE_CXX_STANDARD 17)

include_directories(header)

include_directories(libs)

#lib4aio:
add_subdirectory(libs/lib4aio)

add_executable(osLab main.cpp header/m_client.h source/m_client.cpp header/m_middle_server.h source/m_middle_server.cpp header/util/common.h source/util/common.cpp)

#link lib4aio:
target_link_libraries(osLab lib4aio)