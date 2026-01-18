#ifndef TYPES_H
#define TYPES_H

//rappresenta una posizione (cella) sul grid con coordinate intere x,y
struct Cell {
    int x{};
    int y{};
};

//paragona due celle (inline otimiza chamada da função)
inline bool operator==(const Cell& a, const Cell& b) {
    return a.x == b.x && a.y == b.y;
}

#endif //TYPES_H
