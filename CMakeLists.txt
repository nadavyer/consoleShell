# cmake_minimum_required(VERSION <specify CMake version here>)
project(205709835_204211056)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(205709835_204211056
        include/Commands.h
        include/Environment.h
        include/Files.h
        include/FileSystem.h
        include/GlobalVariables.h
        src/Commands.cpp
        src/Environment.cpp
        src/Files.cpp
        src/FileSystem.cpp
        src/Main.cpp)
