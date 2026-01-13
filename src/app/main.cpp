#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

#include "taikutsu/core/GridMap.h"
#include "taikutsu/core/AStar.h"

constexpr int kCellSize = 25;

static std::optional<Cell> mouseToCell(const sf::RenderWindow& window,
                                       const GridMap& grid) {
    const auto m = sf::Mouse::getPosition(window);
    if (m.x < 0 || m.y < 0) return std::nullopt;

    Cell c{ m.x / kCellSize, m.y / kCellSize };
    if (!grid.inBounds(c)) return std::nullopt;
    return c;
}

static void setTitle(sf::RenderWindow& window, const std::string& status) {
    window.setTitle("Taikutsu - A* | " + status);
}

int main() {
    constexpr int gridW = 30;
    constexpr int gridH = 20;

    sf::RenderWindow window(
        sf::VideoMode(gridW * kCellSize, gridH * kCellSize),
        "Taikutsu - A*"
    );
    window.setFramerateLimit(60);

    GridMap grid(gridW, gridH);
    std::optional<Cell> start;
    std::optional<Cell> goal;

    std::optional<AStarResult> last;
    bool painting = false;                 // LMB pressed
    bool erasing  = false;                 // RMB pressed (opcional)
    std::optional<Cell> lastPainted;       // evita repetir na mesma célula


    sf::RectangleShape cellShape(
        sf::Vector2f(static_cast<float>(kCellSize - 1),
                     static_cast<float>(kCellSize - 1))
    );

    setTitle(window, "LMB paint obstacle | S start | G goal | Space run | R clear result | C clear grid");

    while (window.isOpen()) {
        // célula sob o mouse (hover + comandos S/G)
        const auto hovered = mouseToCell(window, grid);

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // mouse press/release: ativa/desativa pincel (paint)
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    painting = true;
                    lastPainted.reset();
                }
                if (event.mouseButton.button == sf::Mouse::Right) { // opcional
                    erasing = true;
                    lastPainted.reset();
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    painting = false;
                    lastPainted.reset();
                }
                if (event.mouseButton.button == sf::Mouse::Right) { // opcional
                    erasing = false;
                    lastPainted.reset();
                }
            }


            // Teclas
            if (event.type == sf::Event::KeyPressed) {
                // S/G: set start/goal (somente em célula walkable)
                if (event.key.code == sf::Keyboard::S && hovered) {
                    if (grid.isWalkable(*hovered)) {
                        start = *hovered;
                        last.reset();
                    }
                }
                if (event.key.code == sf::Keyboard::G && hovered) {
                    if (grid.isWalkable(*hovered)) {
                        goal = *hovered;
                        last.reset();
                    }
                }

                // space: roda A*
                if (event.key.code == sf::Keyboard::Space) {
                    if (start && goal) {
                        last = AStarPathfinder::findPath(grid, *start, *goal);
                        if (last->success) {
                            setTitle(window, "PATH FOUND | len=" + std::to_string(last->path.size()));
                        } else {
                            setTitle(window, "NO PATH (see explored nodes)");
                        }
                    } else {
                        setTitle(window, "set START (S) and GOAL (G) first");
                    }
                }

                // R: limpa só resultado (path + closed)
                if (event.key.code == sf::Keyboard::R) {
                    last.reset();
                    setTitle(window, "result cleared");
                }

                // C: limpa o grid todo (obstáculos) + resultado
                if (event.key.code == sf::Keyboard::C) {
                    grid = GridMap(gridW, gridH);
                    last.reset();
                    setTitle(window, "grid cleared");
                }

                // Esc: fecha
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // pincel (drag): pinta/apaga enquanto o botão estiver pressionado
        if (hovered) {
            const bool isStart = (start && (*hovered == *start));
            const bool isGoal  = (goal  && (*hovered == *goal));

            if (!isStart && !isGoal) {
                if ((painting || erasing) && (!lastPainted || !(*hovered == *lastPainted))) {

                    if (painting && !grid.isBlocked(*hovered)) {
                        grid.setBlocked(*hovered, true);
                        last.reset();
                    }

                    if (erasing && grid.isBlocked(*hovered)) { // opcional
                        grid.setBlocked(*hovered, false);
                        last.reset();
                    }

                    lastPainted = *hovered;
                }
            }
        }

        window.clear(sf::Color::Black);

        // 1) desenha o grid base (livre/obstáculo + hover)
        for (int y = 0; y < grid.height(); ++y) {
            for (int x = 0; x < grid.width(); ++x) {
                Cell c{x, y};

                if (grid.isBlocked(c)) cellShape.setFillColor(sf::Color(60, 60, 60));
                else                   cellShape.setFillColor(sf::Color(120, 120, 120));

                if (hovered && c == *hovered &&
                    (!start || !(c == *start)) &&
                    (!goal  || !(c == *goal))) {
                    cellShape.setFillColor(sf::Color(180, 180, 180));
                }

                cellShape.setPosition(static_cast<float>(x * kCellSize),
                                      static_cast<float>(y * kCellSize));
                window.draw(cellShape);
            }
        }

        // 2) overlay: explorados (closed) e path (se houver)
        if (last) {
            // explorados (closed) - azul
            cellShape.setFillColor(sf::Color(40, 80, 160));
            for (const auto& c : last->closed) {
                cellShape.setPosition(static_cast<float>(c.x * kCellSize),
                                      static_cast<float>(c.y * kCellSize));
                window.draw(cellShape);
            }

            // caminho final (path) - amarelo
            cellShape.setFillColor(sf::Color(220, 200, 0));
            for (const auto& c : last->path) {
                cellShape.setPosition(static_cast<float>(c.x * kCellSize),
                                      static_cast<float>(c.y * kCellSize));
                window.draw(cellShape);
            }
        }

        // 3) start/goal por cima de tudo
        if (start) {
            cellShape.setFillColor(sf::Color(0, 180, 0));
            cellShape.setPosition(static_cast<float>(start->x * kCellSize),
                                  static_cast<float>(start->y * kCellSize));
            window.draw(cellShape);
        }
        if (goal) {
            cellShape.setFillColor(sf::Color(180, 0, 0));
            cellShape.setPosition(static_cast<float>(goal->x * kCellSize),
                                  static_cast<float>(goal->y * kCellSize));
            window.draw(cellShape);
        }

        window.display();
    }

    return 0;
}
