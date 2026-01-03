#ifndef TYPES_H
#define TYPES_H

#pragma once //preprocessor directive to prevent header files from being included multiple times

struct Cell {
    int x{};
    int y{};
};

inline bool operator==(const Cell& a, const Cell& b) {
    return a.x == b.x && a.y == b.y;
}

#endif //TYPES_H
