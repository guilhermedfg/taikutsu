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
# Tests (GoogleTest)
# ---------------------------
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

# compatibilidade
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

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

target_link_libraries(taikutsu_tests PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(taikutsu_tests)

