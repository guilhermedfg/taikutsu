# Taikutsu — A* Pathfinding (SFML)

Progetto d’esame: movimento su una mappa a griglia con algoritmo A* e visualizzazione tramite SFML.

### Funzionalità
* Griglia 2D con ostacoli modificabili
* Definizione di Start/Goal
* A* (4 direzioni) con euristica Manhattan
* Debug visivo:
    * nodi esplorati (CLOSED)
    * percorso finale
* Unit test (core)

### Controlli
* Mouse sinistro (trascina): disegnare ostacoli
* Mouse destro (trascina): cancellare ostacoli (se abilitato)
* `S`: impostare Start sulla cella sotto il mouse
* `G`: impostare Goal sulla cella sotto il mouse
* `Space`: eseguire A*
* `R`: cancellare il risultato (percorso/nodi esplorati)
* `C`: pulire la griglia (opzionale)
* `esc`: uscire
