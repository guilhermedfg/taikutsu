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

