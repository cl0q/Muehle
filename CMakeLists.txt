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
        src/UIManager/UIManager.h
        src/UIManager/UIManager.cpp
        src/InputHandler/InputHandler.h
        src/InputHandler/InputHandler.cpp
        src/PlayerManager/PlayerManager.h
        src/PlayerManager/PlayerManager.cpp)
