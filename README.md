# Taikutsu — A* Pathfinding (SFML)

Exam project: grid-based movement using the A* pathfinding algorithm with SFML visualization.

## Features
* Editable 2D grid with obstacles
* Start/Goal definition
* A* pathfinding (4-directional) with Manhattan heuristic
* Visual debugging:
    * explored nodes (CLOSED set)
    * final path
* Unit tests (core)

## Controls
* Left mouse button (drag): paint obstacles
* Right mouse button (drag): erase obstacles
* `S`: set Start on the cell 
* `G`: set Goal on the cell 
* `Space`: run A*
* `R`: clear result (path/explored nodes)
* `C`: clear grid (optional)