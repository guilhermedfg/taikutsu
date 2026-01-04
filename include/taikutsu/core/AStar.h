#ifndef ASTAR_H
#define ASTAR_H
#pragma once
#include "GridMap.h"
#include <vector>

struct AStarResult {
    std::vector<Cell> path;    // start -> goal (inclusive)
    std::vector<Cell> closed;  // nós explorados (debug)
    bool success{false};
};

class AStarPathfinder {
public:
    static AStarResult findPath(const GridMap& grid, Cell start, Cell goal);
};


#endif //ASTAR_H
