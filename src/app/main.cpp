#include <SFML/Graphics.hpp>
#include <optional>
#include "taikutsu/core/GridMap.h"

static std::optional<Cell> mouseToCell(const sf::RenderWindow& window, int cellSize, const GridMap& grid) {
    const auto m = sf::Mouse::getPosition(window);
    if (m.x < 0 || m.y < 0) return std::nullopt;

    Cell c{ m.x / cellSize, m.y / cellSize };
    if (!grid.inBounds(c)) return std::nullopt;
    return c;
}

int main() {
    constexpr int cellSize = 25;
    constexpr int gridW = 30;
    constexpr int gridH = 20;

    sf::RenderWindow window(sf::VideoMode(gridW * cellSize, gridH * cellSize), "Taikutsu - Grid");
    window.setFramerateLimit(60);

    GridMap grid(gridW, gridH);
    std::optional<Cell> start;
    std::optional<Cell> goal;

    sf::RectangleShape cellShape(sf::Vector2f(static_cast<float>(cellSize - 1),
                                              static_cast<float>(cellSize - 1)));

    while (window.isOpen()) {
        // célula sob o mouse (para hover e comandos S/G)
        const auto hovered = mouseToCell(window, cellSize, grid);

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Clique esquerdo: toggle obstáculo
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                if (hovered) {
                    // não deixa bloquear start/goal por acidente (opcional, mas evita confusão)
                    if ((!start || *hovered != *start) && (!goal || *hovered != *goal)) {
                        grid.toggleBlocked(*hovered);
                    }
                }
            }

            // Teclas S/G: set start/goal
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S && hovered) {
                    if (grid.isWalkable(*hovered)) start = *hovered;
                }
                if (event.key.code == sf::Keyboard::G && hovered) {
                    if (grid.isWalkable(*hovered)) goal = *hovered;
                }
            }
        }

        window.clear(sf::Color::Black);

        // desenha o grid
        for (int y = 0; y < grid.height(); ++y) {
            for (int x = 0; x < grid.width(); ++x) {
                Cell c{x, y};

                // base (livre vs obstáculo)
                if (grid.isBlocked(c)) {
                    cellShape.setFillColor(sf::Color(60, 60, 60));
                } else {
                    cellShape.setFillColor(sf::Color(120, 120, 120));
                }

                // start/goal por cima
                if (start && c == *start) cellShape.setFillColor(sf::Color(0, 180, 0));
                if (goal  && c == *goal)  cellShape.setFillColor(sf::Color(180, 0, 0));

                // hover (um leve destaque, sem apagar start/goal)
                if (hovered && c == *hovered && (!start || c != *start) && (!goal || c != *goal)) {
                    cellShape.setFillColor(sf::Color(180, 180, 180));
                }

                cellShape.setPosition(static_cast<float>(x * cellSize),
                                      static_cast<float>(y * cellSize));
                window.draw(cellShape);
            }
        }

        window.display();
    }

    return 0;
}
