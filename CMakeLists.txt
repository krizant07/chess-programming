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
    src/piece.cpp
)

# Use if find_package doesnt work, or install ftxui to your system
#include(FetchContent) 
#
#set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
#set(FETCHCONTENT_QUIET OFF)
#
#FetchContent_Declare(ftxui
  #GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  #GIT_TAG        v6.1.9
  #GIT_PROGRESS   TRUE
  #GIT_SHALLOW    TRUE
  #  EXCLUDE_FROM_ALL
  #)
#
#FetchContent_MakeAvailable(ftxui)

find_package(ftxui REQUIRED)

add_executable(board ${SOURCES})

target_compile_options(board PRIVATE -Wall -Werror -pedantic-errors)

target_include_directories(board PRIVATE include)

target_link_libraries(board PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
)
