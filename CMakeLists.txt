cmake_minimum_required(VERSION 3.16)

## Use -std=c++17 as default.
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

project(ppdbscan)

set(SCI_DIR /home/dd/Ubu20/EzPC/SCI/build/install/lib/cmake/SCI)
find_package(SCI REQUIRED)

add_definitions(-w) #不显示warning
add_executable(${PROJECT_NAME} main.cpp func.cpp cloud.cpp)
target_link_libraries(${PROJECT_NAME} SCI::SCI-OT SCI::SCI-HE )
