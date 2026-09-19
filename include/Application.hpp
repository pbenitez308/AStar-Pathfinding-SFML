#pragma once

#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

#include "AStar.hpp"


class Application
{
public:
    Application();

    void run();


private:
    // ========================================================
    // CONFIGURACION
    // ========================================================

    static constexpr int WINDOW_WIDTH = 800;

    static constexpr int GRID_HEIGHT = 600;
    static constexpr int UI_HEIGHT = 90;

    static constexpr int WINDOW_HEIGHT =
        GRID_HEIGHT + UI_HEIGHT;

    static constexpr int CELL_SIZE = 40;

    static constexpr int COLS =
        WINDOW_WIDTH / CELL_SIZE;

    static constexpr int ROWS =
        GRID_HEIGHT / CELL_SIZE;


    // ========================================================
    // SFML
    // ========================================================

    sf::RenderWindow window;

    sf::Font font;

    sf::Text titleText;
    sf::Text controlsText;
    sf::Text statusText;

    sf::RectangleShape uiBar;
    sf::RectangleShape separator;
    sf::RectangleShape statusBox;

    sf::Clock stepClock;


    // ========================================================
    // GRID
    // ========================================================

    std::vector<std::vector<CellState>> grid;


    // ========================================================
    // START / GOAL
    // ========================================================

    bool startPlaced;
    bool goalPlaced;

    Position startPosition;
    Position goalPosition;


    // ========================================================
    // MUROS
    // ========================================================

    bool drawingWalls;
    bool erasingWalls;


    // ========================================================
    // A*
    // ========================================================

    AStar astar;


    // ========================================================
    // FUNCIONES INTERNAS
    // ========================================================

    void processEvents();

    void handleKeyboard(
        const sf::Event::KeyPressed& event
    );

    void handleMousePressed(
        const sf::Event::MouseButtonPressed& event
    );

    void handleMouseReleased(
        const sf::Event::MouseButtonReleased& event
    );

    void handleMouseMoved(
        const sf::Event::MouseMoved& event
    );


    void update();

    void updateAStar();
    void updateStatus();


    void render();

    void drawGrid();
    void drawInterface();


    void resetBoard();

    void clearSearch();


    bool mouseToGrid(
        int mouseX,
        int mouseY,
        int& row,
        int& col
    ) const;
};