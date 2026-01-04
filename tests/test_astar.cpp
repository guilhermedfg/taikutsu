#include "taikutsu/core/AStar.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A* finds a path in empty grid") {
    GridMap grid(5, 5);
    Cell s{0,0};
    Cell g{4,4};

    auto res = AStarPathfinder::findPath(grid, s, g);
    REQUIRE(res.success);
    REQUIRE(res.path.front() == s);
    REQUIRE(res.path.back() == g);

    // em 4-direções, a distância Manhattan é 8 passos, então path tem 9 células
    REQUIRE(res.path.size() == 9);
}

TEST_CASE("A* returns failure when blocked") {
    GridMap grid(5, 5);
    Cell s{0,0};
    Cell g{4,4};

    // cria uma parede vertical bloqueando completamente x=2
    for (int y = 0; y < 5; ++y) grid.setBlocked(Cell{2,y}, true);

    auto res = AStarPathfinder::findPath(grid, s, g);
    REQUIRE_FALSE(res.success);
    REQUIRE(res.path.empty());
}
