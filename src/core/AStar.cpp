#include "taikutsu/core/AStar.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace { //namespace anonimo, tutto è di uso interno di questo .cpp, evita conflitto di nomi e evita accesso esterno

    // Transforma la coppia (x,y) in un valore numerico (hash)
    // usato per trovare velocemente i dati nelle strutture unordered_map e unordered_set (permettere ricerca veloce)
    struct CellHash {
        size_t operator()(const Cell& c) const noexcept {
            // hash simples e suficiente pra grid pequeno/médio
            return (static_cast<size_t>(c.x) << 32) ^ static_cast<size_t>(c.y);
        }
    };

    int manhattan(Cell a, Cell b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y); //abs : valore assoluto
    }

    // ricostruisce la strada finale usando cameFrom (da goal a start)
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

    // Nodo nella coda di priorità
    struct PQNode {
        Cell c; // qualle cella del grid
        int f;  // priorità (f = g + h)
        int g;  // costo reale dallo start fino a questa cella
    };

    // comparatore usato dalla coda di priorità, decide chi ha "piu priorità"
    // return del nodo con valore di f minore
    struct PQCmp {
        bool operator()(const PQNode& a, const PQNode& b) const {
            // priority_queue é max-heap por padrão; invertendo comparação vira min-heap por f
            return a.f > b.f;
        }
    };
}


AStarResult AStarPathfinder::findPath(const GridMap& grid, Cell start, Cell goal) {
    AStarResult result;

    //controllare se start/goal sono all'interno della mappa
    if (!grid.isWalkable(start) || !grid.isWalkable(goal)) return result;

    if (start == goal) {
        result.success = true;
        result.path = {start};
        return result;
    }

    //open set: nodi candidati scoperti a essere espansi/esplorati (priority queue di PQNode)
    std::priority_queue<PQNode, std::vector<PQNode>, PQCmp> open; //PQCmo fa il prossimo candidato essere quello di minore f

    //closed set: insieme di nodi già espansi (per evitare di espandere di nuovo)
    std::unordered_set<Cell, CellHash> closedSet;

    //gScore[c] = miglior costo conosciuto da start fino a c
    std::unordered_map<Cell, int, CellHash> gScore;

    // cameFrom[child] = parent, per ricostruire la strada alla fine
    std::unordered_map<Cell, Cell, CellHash> cameFrom;


    gScore[start] = 0; //il costo per arrivare da start partendo da start é 0
    open.push(PQNode{start, manhattan(start, goal), 0}); //mette start su open set

    while (!open.empty()) { //continua mentre ci sono candidati sulla 'frontiera' open
        PQNode current = open.top();
        open.pop();

        // “skip” entradas desatualizadas, pois no decrease key (porque a gente empilha duplicados)
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
