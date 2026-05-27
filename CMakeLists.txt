cmake_minimum_required(VERSION 3.5)
project(board)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/piece-list.cpp
    src/move.cpp
    src/board.cpp
    src/fen-parser.cpp
    src/move-list.cpp
)

add_executable(board ${SOURCES})

target_include_directories(board
    PRIVATE include
)
