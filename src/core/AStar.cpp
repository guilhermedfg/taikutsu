#include "taikutsu/core/AStar.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>

namespace {
    struct CellHash {
        size_t operator()(const Cell& c) const noexcept {
            // hash simples e suficiente pra grid pequeno/médio
            return (static_cast<size_t>(c.x) << 32) ^ static_cast<size_t>(c.y);
        }
    };

    int manhattan(Cell a, Cell b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    std::vector<Cell> reconstructPath(
        const std::unordered_map<Cell, Cell, CellHash>& cameFrom,
        Cell start, Cell goal
    ) {
        std::vector<Cell> path;
        Cell cur = goal;
        path.push_back(cur);

        while (!(cur == start)) {
            auto it = cameFrom.find(cur);
            if (it == cameFrom.end()) {
                // não deveria acontecer se success=true, mas evita loop infinito
                return {};
            }
            cur = it->second;
            path.push_back(cur);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    struct PQNode {
        Cell c;
        int f;
        int g;
    };

    struct PQCmp {
        bool operator()(const PQNode& a, const PQNode& b) const {
            // priority_queue é max-heap; invertendo vira min-heap por f
            return a.f > b.f;
        }
    };
}

AStarResult AStarPathfinder::findPath(const GridMap& grid, Cell start, Cell goal) {
    AStarResult result;

    if (!grid.isWalkable(start) || !grid.isWalkable(goal)) return result;

    if (start == goal) {
        result.success = true;
        result.path = {start};
        return result;
    }

    std::priority_queue<PQNode, std::vector<PQNode>, PQCmp> open;
    std::unordered_set<Cell, CellHash> closedSet;

    std::unordered_map<Cell, int, CellHash> gScore;
    std::unordered_map<Cell, Cell, CellHash> cameFrom;

    gScore[start] = 0;
    open.push(PQNode{start, manhattan(start, goal), 0});

    while (!open.empty()) {
        PQNode current = open.top();
        open.pop();

        // “skip” entradas desatualizadas (porque a gente empilha duplicados)
        auto itG = gScore.find(current.c);
        if (itG == gScore.end() || current.g != itG->second) continue;

        if (closedSet.contains(current.c)) continue;

        // marca como explorado
        closedSet.insert(current.c);
        result.closed.push_back(current.c);

        if (current.c == goal) {
            result.success = true;
            result.path = reconstructPath(cameFrom, start, goal);
            return result;
        }

        for (Cell nb : grid.neighbors4(current.c)) {
            if (closedSet.contains(nb)) continue;

            const int tentativeG = current.g + 1; // custo unitário

            auto it = gScore.find(nb);
            const bool better = (it == gScore.end()) || tentativeG < it->second;

            if (better) {
                cameFrom[nb] = current.c;
                gScore[nb] = tentativeG;
                const int f = tentativeG + manhattan(nb, goal);
                open.push(PQNode{nb, f, tentativeG});
            }
        }
    }

    // sem caminho
    return result;
}
