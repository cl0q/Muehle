cmake_minimum_required(VERSION 3.30)
project(Muehle)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Muehle
        src/BoardManager/BoardManager.h
        src/main.cpp
        src/BoardManager/BoardManager.cpp
        src/Logger/Logger.cpp
        src/GameManager/GameManager.h
        src/GameManager/GameManager.cpp
        src/InputHandler/InputHandler.cpp
        src/InputHandler/InputHandler.h)
