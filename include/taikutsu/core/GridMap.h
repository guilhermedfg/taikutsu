#ifndef GRIDMAP_H
#define GRIDMAP_H


#include "Types.h" //cell (x,y)
#include <vector>

//Qui rappresentiamo il nostro gridmap
class GridMap {
public:
    GridMap(int width, int height); //larghezza x altezza dell'intero grid (ctor)

    //getters
    int width() const { return w_; }
    int height() const { return h_; }

    //controlli logici
    bool inBounds(Cell c) const;
    bool isBlocked(Cell c) const;
    bool isWalkable(Cell c) const { return inBounds(c) && !isBlocked(c); }

    //modifica stato griglia
    void setBlocked(Cell c, bool blocked);
    void toggleBlocked(Cell c);

    // Para o A*: retorna os vizinhos em 4 direções (apenas walkable)
    std::vector<Cell> neighbors4(Cell c) const;

private:
    //dimensioni del grid
    int w_{};
    int h_{};

    // Armazenamento 1D: blocked_[idx] diz se a célula é obstáculo.
    std::vector<bool> blocked_; // true=obstacle, false=libero (blocked tem tamanho w_ * h_)

    //idx = y * width + x
    // cada valor inteiro (1D) desse corresponde a uma dupla de células (x,y)
    // ex. blocked_ [13] = (3,2) -> idx=2*5+3 com (w_=5)
    //transforma coordinate 2d in indice 1d
    int idx(Cell c) const { return c.y * w_ + c.x; }
};

#endif // GRIDMAP_H
