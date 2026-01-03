cmake_minimum_required(VERSION 3.20)
project(taikutsu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(taikutsu_app
        src/app/main.cpp
        src/core/GridMap.cpp
)

target_include_directories(taikutsu_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(taikutsu_app PRIVATE -Wall -Wextra -Wpedantic)

find_package(SFML 2 CONFIG REQUIRED COMPONENTS graphics window system)
target_link_libraries(taikutsu_app PRIVATE sfml-graphics sfml-window sfml-system)
