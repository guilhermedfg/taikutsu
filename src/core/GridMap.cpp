#include "taikutsu/core/GridMap.h"

GridMap::GridMap(int width, int height)
    : w_(width), h_(height), blocked_(static_cast<size_t>(width * height), false) {}

bool GridMap::inBounds(Cell c) const {
    return c.x >= 0 && c.x < w_ && c.y >= 0 && c.y < h_;
}

bool GridMap::isBlocked(Cell c) const {
    if (!inBounds(c)) return true;
    return blocked_[static_cast<size_t>(idx(c))];
}

void GridMap::setBlocked(Cell c, bool blocked) {
    if (!inBounds(c)) return;
    blocked_[static_cast<size_t>(idx(c))] = blocked;
}

void GridMap::toggleBlocked(Cell c) {
    if (!inBounds(c)) return;
    const auto i = static_cast<size_t>(idx(c));
    blocked_[i] = !blocked_[i];
}

std::vector<Cell> GridMap::neighbors4(Cell c) const {
    std::vector<Cell> out;
    out.reserve(4);

    const Cell cand[4] = {
        {c.x + 1, c.y},
        {c.x - 1, c.y},
        {c.x, c.y + 1},
        {c.x, c.y - 1}
    };

    for (Cell n : cand) {
        if (isWalkable(n)) out.push_back(n);
    }

    return out;
}
