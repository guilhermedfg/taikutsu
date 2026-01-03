#ifndef GRIDMAP_H
#define GRIDMAP_H
#pragma once
#include "Types.h"
#include <vector>

class GridMap {
public:
    GridMap(int width, int height);

    int width() const { return w_; }
    int height() const { return h_; }

    bool inBounds(Cell c) const;
    bool isBlocked(Cell c) const;
    bool isWalkable(Cell c) const { return inBounds(c) && !isBlocked(c); }

    void setBlocked(Cell c, bool blocked);
    void toggleBlocked(Cell c);

private:
    int w_{};
    int h_{};
    std::vector<bool> blocked_; // true = obstacle

    int idx(Cell c) const { return c.y * w_ + c.x; }
};

#endif //GRIDMAP_H
