#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "AStar.hpp"


class Grid
{
public:
    // ========================================================
    // CONFIGURACION DEL GRID
    // ========================================================

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    static constexpr int CELL_SIZE = 40;

    static constexpr int COLS =
        WIDTH / CELL_SIZE;

    static constexpr int ROWS =
        HEIGHT / CELL_SIZE;


    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    explicit Grid(int topOffset);


    // ========================================================
    // INPUT
    // ========================================================

    void handleMousePressed(
        const sf::Event::MouseButtonPressed& event
    );

    void handleMouseReleased(
        const sf::Event::MouseButtonReleased& event
    );

    void handleMouseMoved(
        const sf::Event::MouseMoved& event
    );


    // ========================================================
    // TABLERO
    // ========================================================

    void reset();

    void clearSearch();


    // ========================================================
    // RENDER
    // ========================================================

    void draw(
        sf::RenderWindow& window
    ) const;


    // ========================================================
    // INFORMACION START / GOAL
    // ========================================================

    bool hasStart() const;

    bool hasGoal() const;

    Position getStartPosition() const;

    Position getGoalPosition() const;


    // ========================================================
    // ACCESO PARA A*
    // ========================================================

    std::vector<std::vector<CellState>>& cells();

    const std::vector<std::vector<CellState>>& cells() const;


private:
    // ========================================================
    // DATOS
    // ========================================================

    int topOffset;


    std::vector<std::vector<CellState>> grid;


    bool startPlaced;
    bool goalPlaced;


    Position startPosition;
    Position goalPosition;


    bool drawingWalls;
    bool erasingWalls;


    // ========================================================
    // UTILIDADES
    // ========================================================

    bool mouseToGrid(
        int mouseX,
        int mouseY,
        int& row,
        int& col
    ) const;
};