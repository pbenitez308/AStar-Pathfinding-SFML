#pragma once

#include <SFML/Graphics.hpp>

#include "AStar.hpp"
#include "Grid.hpp"


class Application
{
public:
    Application();

    void run();


private:
    // ========================================================
    // CONFIGURACION
    // ========================================================

    static constexpr int UI_HEIGHT = 90;

    static constexpr int WINDOW_WIDTH =
        Grid::WIDTH;

    static constexpr int WINDOW_HEIGHT =
        Grid::HEIGHT + UI_HEIGHT;


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
    // COMPONENTES
    // ========================================================

    Grid grid;

    AStar astar;


    // ========================================================
    // EVENTOS
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


    // ========================================================
    // UPDATE
    // ========================================================

    void update();

    void updateAStar();

    void updateStatus();


    // ========================================================
    // RENDER
    // ========================================================

    void render();

    void drawInterface();
};