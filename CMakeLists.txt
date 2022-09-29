cmake_minimum_required(VERSION 3.16)

## Use -std=c++17 as default.
set(CMAKE_CXX_STANDARD 17)
## Disable C++ extensions
set(CMAKE_CXX_EXTENSIO  tandard)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

project(ppdbscan)

set(SCI_DIR /home/dd/Ubu20/sskdmeans/install/lib/cmake/SCI)
find_package(SCI REQUIRED)

# add_executable(${PROJECT_NAME} ss.cpp cloud.cpp cloud_one.cpp cloud_two.cpp func.cpp ${BACKWARD_ENABLE})
add_executable(${PROJECT_NAME} main.cpp func.cpp cloud.cpp)

# add_backward(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} SCI::SCI-OT SCI::SCI-HE )
