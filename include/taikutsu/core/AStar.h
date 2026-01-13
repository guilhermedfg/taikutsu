#ifndef ASTAR_H
#define ASTAR_H

#include "GridMap.h"
#include <vector>

// risultato A*
struct AStarResult {
    // Strada percorsa, da start fino a goal (if sucecess=false, no path)
    std::vector<Cell> path;    //start -> goal (inclusive)
    std::vector<Cell> closed;  //nodi esplorati (debug) utile per colorarli
    bool success{false}; //indica se ha trovato strada da percorrere
};

// Interfaccia A* (classe stateless, non imagazzina stato interno, offre solo funzione pura)
class AStarPathfinder {
public:
    // Esegue A* sul grid, cercando strada tra start/goal
    // return di AStarResult con il path disegnato
    static AStarResult findPath(const GridMap& grid, Cell start, Cell goal);
};


#endif //ASTAR_H
