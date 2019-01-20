cmake_minimum_required(VERSION 3.0.0)

project(hangman)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(SRCS
    main.cpp
    hangman.ui
    hangman.cpp
)

add_executable(hangman ${SRCS})

target_link_libraries(hangman Qt5::Widgets Qt5::Network)
