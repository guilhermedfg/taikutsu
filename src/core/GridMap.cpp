#include "taikutsu/core/GridMap.h"

// costruisce grid WxH e inizializza tutte le celle come libere (false)
GridMap::GridMap(int width, int height)
    : w_(width), h_(height), blocked_(static_cast<size_t>(width * height), false) {}

// controlla se una coordinata (x,y) è all'interno dei limiti del grid
bool GridMap::inBounds(Cell c) const {
    return c.x >= 0 && c.x < w_ && c.y >= 0 && c.y < h_;
}

// fa return se una cella è obstacle oppure no.
// "off bounds"/fuori mappa è considerato come obstacle,
// ciò semplifica A*, può usare is Walkable senza preocuparsi con boundaries
bool GridMap::isBlocked(Cell c) const {
    if (!inBounds(c)) return true;
    return blocked_[static_cast<size_t>(idx(c))];
}

// determina se una cella è bloccata oppure no
// se è fuori mappa, la ignora
void GridMap::setBlocked(Cell c, bool blocked) {
    if (!inBounds(c)) return;
    blocked_[static_cast<size_t>(idx(c))] = blocked;
}

//alterna lo stato: Libero diventa obstacle, e Obstacle diventa libero
// utile per modifica del gridmap tramite click del mouse
void GridMap::toggleBlocked(Cell c) {
    if (!inBounds(c)) return;
    const auto i = static_cast<size_t>(idx(c));
    blocked_[i] = !blocked_[i];
}

// return dei vicini in 4 direzioni (no diagonali)
std::vector<Cell> GridMap::neighbors4(Cell c) const {
    std::vector<Cell> out;
    out.reserve(4);

    const Cell cand[4] = {
        {c.x + 1, c.y},  // candidati in 4 direzioni (right, left, up, down)
        {c.x - 1, c.y},
        {c.x, c.y + 1},
        {c.x, c.y - 1}
    };

    //filtra solo i candidati che sono walkable (non bloccati)
    for (Cell n : cand) {
        if (isWalkable(n)) out.push_back(n);
    }

    return out;
}
