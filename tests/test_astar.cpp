// tests/test_astar.cpp
#include <gtest/gtest.h>
#include <cmath>

#include "taikutsu/core/AStar.h"
#include "taikutsu/core/GridMap.h"
#include "taikutsu/core/Types.h"

// ===================== helpers =====================

static int manhattan(Cell a, Cell b) { //manhattan 4 direzioni
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// controlla se il path non è vuoto, va da start a goal,
//tutte le celle sono all'interno del grid e walkable e uso corretto di manhattan
static void assertValidPath(const GridMap& g,
                            const std::vector<Cell>& path,
                            Cell start,
                            Cell goal) {
    ASSERT_FALSE(path.empty());
    EXPECT_TRUE(path.front() == start);
    EXPECT_TRUE(path.back() == goal);

    for (size_t i = 0; i < path.size(); ++i) {
        const Cell c = path[i];
        EXPECT_TRUE(g.inBounds(c)) << "Cell out of bounds: (" << c.x << "," << c.y << ")";
        EXPECT_TRUE(g.isWalkable(c)) << "Cell not walkable: (" << c.x << "," << c.y << ")";

        if (i > 0) {
            const Cell prev = path[i - 1];
            EXPECT_EQ(manhattan(prev, c), 1)
                << "Non-4-neighbor step from (" << prev.x << "," << prev.y << ") to ("
                << c.x << "," << c.y << ")";
        }
    }
}

// ===================== tests =====================

//1.Test start=goal
TEST(AStar, StartEqualsGoal_ReturnsSingleCellPath) {
    GridMap g(5, 5);
    Cell s{2, 2};

    AStarResult res = AStarPathfinder::findPath(g, s, s);

    EXPECT_TRUE(res.success);
    ASSERT_EQ(res.path.size(), 1u);
    EXPECT_TRUE(res.path.front() == s);
    EXPECT_TRUE(res.path.back() == s);
}

//2.Test grid vuoto
TEST(AStar, EmptyGrid_ShortestPathLengthIsManhattanPlusOne) {
    GridMap g(6, 6);
    Cell s{0, 0};
    Cell t{5, 3};

    AStarResult res = AStarPathfinder::findPath(g, s, t);

    EXPECT_TRUE(res.success);
    assertValidPath(g, res.path, s, t);

    const int expectedCells = manhattan(s, t) + 1; // conta le celle (inclusi start e goal)
    EXPECT_EQ(static_cast<int>(res.path.size()), expectedCells);
}

//test ostacolo semplice
TEST(AStar, BlockedCells_ForceDetourPathIsLongerThanManhattan) {
    GridMap g(6, 3);
    Cell s{0, 1};
    Cell t{5, 1};

    // blocca due celle in mezzo, obbligando spostamento in cima/basso
    g.setBlocked(Cell{2, 1}, true);
    g.setBlocked(Cell{3, 1}, true);

    AStarResult res = AStarPathfinder::findPath(g, s, t);

    EXPECT_TRUE(res.success);
    assertValidPath(g, res.path, s, t);

    const int minCells = manhattan(s, t) + 1;
    EXPECT_GT(static_cast<int>(res.path.size()), minCells);
}
//4.Test parete verticale
TEST(AStar, NoPath_WhenVerticalWallSplitsGrid) {
    GridMap g(6, 3);
    Cell s{1, 1};
    Cell t{5, 1};

    // parete verticale in x=3, tagliando il grid in 2
    for (int y = 0; y < g.height(); ++y) {
        g.setBlocked(Cell{3, y}, true);
    }

    AStarResult res = AStarPathfinder::findPath(g, s, t);

    EXPECT_FALSE(res.success);
    EXPECT_TRUE(res.path.empty());
}

//5.Test start o goal bloccati
TEST(AStar, StartOrGoalBlocked_ReturnsFailure) {
    GridMap g(5, 5);
    Cell s{0, 0};
    Cell t{4, 4};

    // start bloccato
    g.setBlocked(s, true);
    {
        AStarResult res = AStarPathfinder::findPath(g, s, t);
        EXPECT_FALSE(res.success);
        EXPECT_TRUE(res.path.empty());
    }

    // goal bloccato
    g.setBlocked(s, false);
    g.setBlocked(t, true);
    {
        AStarResult res = AStarPathfinder::findPath(g, s, t);
        EXPECT_FALSE(res.success);
        EXPECT_TRUE(res.path.empty());
    }
}

//6.Test coordinate invalide (offbounds) di start o goal
TEST(AStar, StartOrGoalOutOfBounds_ReturnsFailure) {
    GridMap g(5, 5);

    // start fuori bound
    {
        AStarResult res = AStarPathfinder::findPath(g, Cell{-1, 0}, Cell{2, 2});
        EXPECT_FALSE(res.success);
        EXPECT_TRUE(res.path.empty());
    }

    // goal fuori bound
    {
        AStarResult res = AStarPathfinder::findPath(g, Cell{0, 0}, Cell{99, 2});
        EXPECT_FALSE(res.success);
        EXPECT_TRUE(res.path.empty());
    }
}

//7. ostacoli vari sul grid con possibilità di costruzione path
TEST(AStar, PathNeverStepsOnObstacles) {
    GridMap g(8, 8);
    Cell s{0, 0};
    Cell t{7, 7};

    // crea alcuni ostacoli (senza bloccare totalmente)
    g.setBlocked(Cell{1, 0}, true);
    g.setBlocked(Cell{1, 1}, true);
    g.setBlocked(Cell{2, 1}, true);
    g.setBlocked(Cell{3, 3}, true);
    g.setBlocked(Cell{4, 3}, true);
    g.setBlocked(Cell{4, 4}, true);

    AStarResult res = AStarPathfinder::findPath(g, s, t);

    EXPECT_TRUE(res.success);
    assertValidPath(g, res.path, s, t);
}
