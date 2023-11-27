project(WSCHATClient)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} 
        PUBLIC 
        WSCHATNetwork
)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
        WSCHATNetwork
        ${CMAKE_THREAD_LIBS_INIT}
)