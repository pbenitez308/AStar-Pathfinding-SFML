#include <SFML/Graphics.hpp>
#include <vector>

#include "AStar.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int CELL_SIZE = 40;

const int COLS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = WINDOW_HEIGHT / CELL_SIZE;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "A* PathFinding");

    std::vector<std::vector<CellState>> grid(
        ROWS,
        std::vector<CellState>(COLS, CellState::Empty));

    bool startPlaced = false;
    bool goalPlaced = false;

    Position startPosition{-1, -1};
    Position goalPosition{-1, -1};

    bool drawingWalls = false;
    bool erasingWalls = false;

    AStar astar;

    sf::Clock stepClock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Cerrar ventana
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Teclado
            if (const auto *keyEvent =
                    event->getIf<sf::Event::KeyPressed>())
            {
                // R -> Reiniciar tablero
                if (keyEvent->code == sf::Keyboard::Key::R)
                {
                    for (int row = 0; row < ROWS; row++)
                    {
                        for (int col = 0; col < COLS; col++)
                        {
                            grid[row][col] = CellState::Empty;
                        }
                    }

                    startPlaced = false;
                    goalPlaced = false;

                    startPosition = {-1, -1};
                    goalPosition = {-1, -1};

                    astar.reset();
                }

                // SPACE -> Ejecutar A*
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (startPlaced &&
                        goalPlaced &&
                        !astar.isRunning())
                    {
                        astar.begin(
                            grid,
                            startPosition,
                            goalPosition);

                        stepClock.restart();
                    }
                }

                // C -> Limpiar resultados de la búsqueda
                if (keyEvent->code == sf::Keyboard::Key::C)
                {
                    for (int row = 0; row < ROWS; row++)
                    {
                        for (int col = 0; col < COLS; col++)
                        {
                            if (grid[row][col] == CellState::Open ||
                                grid[row][col] == CellState::Closed ||
                                grid[row][col] == CellState::Path)
                            {
                                grid[row][col] = CellState::Empty;
                            }
                        }
                    }

                    astar.reset();
                }
            }

            // Presionar botón del mouse
            if (const auto *mouseEvent =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                int col = mouseEvent->position.x / CELL_SIZE;
                int row = mouseEvent->position.y / CELL_SIZE;

                if (row >= 0 && row < ROWS &&
                    col >= 0 && col < COLS)
                {
                    // Izquierdo -> Start
                    if (mouseEvent->button == sf::Mouse::Button::Left)
                    {
                        if (!startPlaced &&
                            grid[row][col] == CellState::Empty)
                        {
                            grid[row][col] = CellState::Start;

                            startPosition = {row, col};

                            startPlaced = true;
                        }
                    }

                    // Derecho -> Goal
                    if (mouseEvent->button == sf::Mouse::Button::Right)
                    {
                        if (!goalPlaced &&
                            grid[row][col] == CellState::Empty)
                        {
                            grid[row][col] = CellState::Goal;

                            goalPosition = {row, col};

                            goalPlaced = true;
                        }
                    }

                    // Central -> comenzar a dibujar muros
                    if (mouseEvent->button == sf::Mouse::Button::Middle)
                    {
                        drawingWalls = true;

                        // Si hacemos clic sobre un muro,
                        // entramos en modo borrar.
                        if (grid[row][col] == CellState::Wall)
                        {
                            erasingWalls = true;
                            grid[row][col] = CellState::Empty;
                        }

                        // Si hacemos clic sobre una celda vacía,
                        // entramos en modo dibujar.
                        else if (grid[row][col] == CellState::Empty)
                        {
                            erasingWalls = false;
                            grid[row][col] = CellState::Wall;
                        }
                    }
                }
            }

            // Soltar botón central
            if (const auto *mouseEvent =
                    event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == sf::Mouse::Button::Middle)
                {
                    drawingWalls = false;
                    erasingWalls = false;
                }
            }

            // Arrastrar para dibujar muros
            if (const auto *moveEvent =
                    event->getIf<sf::Event::MouseMoved>())
            {
                if (drawingWalls)
                {
                    int col = moveEvent->position.x / CELL_SIZE;
                    int row = moveEvent->position.y / CELL_SIZE;

                    if (row >= 0 && row < ROWS &&
                        col >= 0 && col < COLS)
                    {
                        // Modo borrar
                        if (erasingWalls)
                        {
                            if (grid[row][col] == CellState::Wall)
                            {
                                grid[row][col] = CellState::Empty;
                            }
                        }

                        // Modo dibujar
                        else
                        {
                            if (grid[row][col] == CellState::Empty)
                            {
                                grid[row][col] = CellState::Wall;
                            }
                        }
                    }
                }
            }
        }

        // Ejecutar un paso de A* cada 40 ms
        if (astar.isRunning())
        {
            if (stepClock.getElapsedTime().asMilliseconds() >= 40)
            {
                astar.step(grid);
                stepClock.restart();
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // Dibujar cuadrícula
        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                sf::RectangleShape cell(
                    sf::Vector2f(
                        static_cast<float>(CELL_SIZE - 1),
                        static_cast<float>(CELL_SIZE - 1)));

                cell.setPosition(
                    sf::Vector2f(
                        static_cast<float>(col * CELL_SIZE),
                        static_cast<float>(row * CELL_SIZE)));

                switch (grid[row][col])
                {
                case CellState::Empty:
                    cell.setFillColor(sf::Color(50, 50, 50));
                    break;

                case CellState::Start:
                    cell.setFillColor(sf::Color::Green);
                    break;

                case CellState::Goal:
                    cell.setFillColor(sf::Color::Red);
                    break;

                case CellState::Wall:
                    cell.setFillColor(sf::Color::Black);
                    break;

                case CellState::Open:
                    cell.setFillColor(sf::Color(0, 150, 255));
                    break;

                case CellState::Closed:
                    cell.setFillColor(sf::Color(100, 100, 255));
                    break;

                case CellState::Path:
                    cell.setFillColor(sf::Color::Yellow);
                    break;
                }

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}