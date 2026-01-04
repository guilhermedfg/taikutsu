cmake_minimum_required(VERSION 3.20)
project(taikutsu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# App (SFML)
# -----------------------------
add_executable(taikutsu_app
        src/app/main.cpp
        src/core/GridMap.cpp
        src/core/AStar.cpp
)

target_include_directories(taikutsu_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(taikutsu_app PRIVATE -Wall -Wextra -Wpedantic)

find_package(SFML 2 CONFIG REQUIRED COMPONENTS graphics window system)
target_link_libraries(taikutsu_app PRIVATE sfml-graphics sfml-window sfml-system)

# -----------------------------
# Tests (Catch2)
# -----------------------------
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(taikutsu_tests
        tests/test_astar.cpp
        src/core/GridMap.cpp
        src/core/AStar.cpp
)

target_include_directories(taikutsu_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(taikutsu_tests PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(taikutsu_tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME taikutsu_tests COMMAND taikutsu_tests)

