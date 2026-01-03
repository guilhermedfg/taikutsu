cmake_minimum_required(VERSION 3.20)
project(taikutsu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(taikutsu_app
    src/app/main.cpp
)

target_compile_options(taikutsu_app PRIVATE -Wall -Wextra -Wpedantic)

# Homebrew (Apple Silicon): /opt/homebrew/opt/sfml@2
# (Intel): /usr/local/opt/sfml@2
# vc pode passar isso via -DCMAKE_PREFIX_PATH também (recomendado).
find_package(SFML 2 CONFIG REQUIRED COMPONENTS graphics window system)

# SFML 2 normalmente expõe sfml-graphics etc.
target_link_libraries(taikutsu_app PRIVATE sfml-graphics sfml-window sfml-system)
