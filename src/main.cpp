#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

#include "AStar.hpp"

// ==============================
// CONFIGURACION DE LA VENTANA
// ==============================

const int WINDOW_WIDTH = 800;

const int GRID_HEIGHT = 600;
const int UI_HEIGHT = 60;

const int WINDOW_HEIGHT = GRID_HEIGHT + UI_HEIGHT;

const int CELL_SIZE = 40;

const int COLS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = GRID_HEIGHT / CELL_SIZE;


int main()
{
    // ==============================
    // VENTANA
    // ==============================

    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "A* PathFinding"
    );


    // ==============================
    // FUENTE E INTERFAZ
    // ==============================

    sf::Font font("assets/Roboto-Regular.ttf");


    sf::Text controlsText(
        font,
        "LMB: Start   RMB: Goal   MMB: Walls   SPACE: Run   C: Clear   R: Reset",
        14
    );

    controlsText.setPosition({15.f, 8.f});


    sf::Text statusText(
        font,
        "STATUS: READY",
        15
    );

    statusText.setPosition({15.f, 34.f});


    // Barra superior
    sf::RectangleShape uiBar(
        sf::Vector2f(
            static_cast<float>(WINDOW_WIDTH),
            static_cast<float>(UI_HEIGHT)
        )
    );

    uiBar.setPosition({0.f, 0.f});
    uiBar.setFillColor(sf::Color(20, 20, 20));


    // ==============================
    // GRID
    // ==============================

    std::vector<std::vector<CellState>> grid(
        ROWS,
        std::vector<CellState>(
            COLS,
            CellState::Empty
        )
    );


    bool startPlaced = false;
    bool goalPlaced = false;

    Position startPosition{-1, -1};
    Position goalPosition{-1, -1};


    // ==============================
    // MUROS
    // ==============================

    bool drawingWalls = false;
    bool erasingWalls = false;


    // ==============================
    // A*
    // ==============================

    AStar astar;

    sf::Clock stepClock;


    // ==============================
    // LOOP PRINCIPAL
    // ==============================

    while (window.isOpen())
    {
        // ==============================
        // EVENTOS
        // ==============================

        while (const std::optional event = window.pollEvent())
        {
            // --------------------------
            // CERRAR VENTANA
            // --------------------------

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }


            // ==============================
            // TECLADO
            // ==============================

            if (const auto* keyEvent =
                    event->getIf<sf::Event::KeyPressed>())
            {
                // --------------------------
                // R -> RESET COMPLETO
                // --------------------------

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

                    drawingWalls = false;
                    erasingWalls = false;

                    astar.reset();
                }


                // --------------------------
                // SPACE -> EJECUTAR A*
                // --------------------------

                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (
                        startPlaced &&
                        goalPlaced &&
                        !astar.isRunning()
                    )
                    {
                        astar.begin(
                            grid,
                            startPosition,
                            goalPosition
                        );

                        stepClock.restart();
                    }
                }


                // --------------------------
                // C -> LIMPIAR BUSQUEDA
                // --------------------------

                if (keyEvent->code == sf::Keyboard::Key::C)
                {
                    for (int row = 0; row < ROWS; row++)
                    {
                        for (int col = 0; col < COLS; col++)
                        {
                            if (
                                grid[row][col] == CellState::Open ||
                                grid[row][col] == CellState::Closed ||
                                grid[row][col] == CellState::Path
                            )
                            {
                                grid[row][col] = CellState::Empty;
                            }
                        }
                    }

                    astar.reset();
                }
            }


            // ==============================
            // MOUSE PRESIONADO
            // ==============================

            if (const auto* mouseEvent =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                // No permitimos modificar el tablero
                // mientras A* esta ejecutandose
                if (
                    !astar.isRunning() &&
                    mouseEvent->position.y >= UI_HEIGHT
                )
                {
                    int col =
                        mouseEvent->position.x /
                        CELL_SIZE;

                    int row =
                        (mouseEvent->position.y - UI_HEIGHT) /
                        CELL_SIZE;


                    // Verificar limites
                    if (
                        row >= 0 &&
                        row < ROWS &&
                        col >= 0 &&
                        col < COLS
                    )
                    {
                        // --------------------------
                        // CLICK IZQUIERDO -> START
                        // --------------------------

                        if (
                            mouseEvent->button ==
                            sf::Mouse::Button::Left
                        )
                        {
                            if (
                                !startPlaced &&
                                grid[row][col] ==
                                    CellState::Empty
                            )
                            {
                                grid[row][col] =
                                    CellState::Start;

                                startPosition =
                                    {row, col};

                                startPlaced = true;
                            }
                        }


                        // --------------------------
                        // CLICK DERECHO -> GOAL
                        // --------------------------

                        if (
                            mouseEvent->button ==
                            sf::Mouse::Button::Right
                        )
                        {
                            if (
                                !goalPlaced &&
                                grid[row][col] ==
                                    CellState::Empty
                            )
                            {
                                grid[row][col] =
                                    CellState::Goal;

                                goalPosition =
                                    {row, col};

                                goalPlaced = true;
                            }
                        }


                        // --------------------------
                        // CLICK CENTRAL -> MUROS
                        // --------------------------

                        if (
                            mouseEvent->button ==
                            sf::Mouse::Button::Middle
                        )
                        {
                            // Click sobre muro:
                            // comenzar a borrar
                            if (
                                grid[row][col] ==
                                CellState::Wall
                            )
                            {
                                drawingWalls = true;
                                erasingWalls = true;

                                grid[row][col] =
                                    CellState::Empty;
                            }

                            // Click sobre espacio vacio:
                            // comenzar a dibujar
                            else if (
                                grid[row][col] ==
                                CellState::Empty
                            )
                            {
                                drawingWalls = true;
                                erasingWalls = false;

                                grid[row][col] =
                                    CellState::Wall;
                            }

                            // Start, Goal, Path, etc.
                            // no pueden convertirse en muro
                            else
                            {
                                drawingWalls = false;
                            }
                        }
                    }
                }
            }


            // ==============================
            // SOLTAR BOTON CENTRAL
            // ==============================

            if (const auto* mouseEvent =
                    event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (
                    mouseEvent->button ==
                    sf::Mouse::Button::Middle
                )
                {
                    drawingWalls = false;
                    erasingWalls = false;
                }
            }


            // ==============================
            // ARRASTRAR MUROS
            // ==============================

            if (const auto* moveEvent =
                    event->getIf<sf::Event::MouseMoved>())
            {
                if (
                    drawingWalls &&
                    !astar.isRunning() &&
                    moveEvent->position.y >= UI_HEIGHT
                )
                {
                    int col =
                        moveEvent->position.x /
                        CELL_SIZE;

                    int row =
                        (moveEvent->position.y - UI_HEIGHT) /
                        CELL_SIZE;


                    if (
                        row >= 0 &&
                        row < ROWS &&
                        col >= 0 &&
                        col < COLS
                    )
                    {
                        // --------------------------
                        // MODO BORRAR
                        // --------------------------

                        if (erasingWalls)
                        {
                            if (
                                grid[row][col] ==
                                CellState::Wall
                            )
                            {
                                grid[row][col] =
                                    CellState::Empty;
                            }
                        }

                        // --------------------------
                        // MODO DIBUJAR
                        // --------------------------

                        else
                        {
                            if (
                                grid[row][col] ==
                                CellState::Empty
                            )
                            {
                                grid[row][col] =
                                    CellState::Wall;
                            }
                        }
                    }
                }
            }
        }


        // ==============================
        // EJECUTAR A* PASO A PASO
        // ==============================

        if (astar.isRunning())
        {
            if (
                stepClock
                    .getElapsedTime()
                    .asMilliseconds() >= 40
            )
            {
                astar.step(grid);

                stepClock.restart();
            }
        }


        // ==============================
        // ACTUALIZAR STATUS
        // ==============================

        if (astar.isRunning())
        {
            statusText.setString(
                "STATUS: SEARCHING..."
            );
        }
        else if (astar.pathFound())
        {
            statusText.setString(
                "STATUS: PATH FOUND"
            );
        }
        else
        {
            statusText.setString(
                "STATUS: READY"
            );
        }


        // ==============================
        // RENDER
        // ==============================

        window.clear(
            sf::Color(30, 30, 30)
        );


        // Barra superior
        window.draw(uiBar);


        // ==============================
        // DIBUJAR CUADRICULA
        // ==============================

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                sf::RectangleShape cell(
                    sf::Vector2f(
                        static_cast<float>(
                            CELL_SIZE - 1
                        ),
                        static_cast<float>(
                            CELL_SIZE - 1
                        )
                    )
                );


                // IMPORTANTE:
                // + UI_HEIGHT mueve el grid
                // debajo de la barra superior.
                cell.setPosition(
                    sf::Vector2f(
                        static_cast<float>(
                            col * CELL_SIZE
                        ),
                        static_cast<float>(
                            row * CELL_SIZE +
                            UI_HEIGHT
                        )
                    )
                );


                // ==============================
                // COLOR DE CELDA
                // ==============================

                switch (grid[row][col])
                {
                    case CellState::Empty:
                        cell.setFillColor(
                            sf::Color(50, 50, 50)
                        );
                        break;

                    case CellState::Start:
                        cell.setFillColor(
                            sf::Color::Green
                        );
                        break;

                    case CellState::Goal:
                        cell.setFillColor(
                            sf::Color::Red
                        );
                        break;

                    case CellState::Wall:
                        cell.setFillColor(
                            sf::Color::Black
                        );
                        break;

                    case CellState::Open:
                        cell.setFillColor(
                            sf::Color(0, 150, 255)
                        );
                        break;

                    case CellState::Closed:
                        cell.setFillColor(
                            sf::Color(100, 100, 255)
                        );
                        break;

                    case CellState::Path:
                        cell.setFillColor(
                            sf::Color::Yellow
                        );
                        break;
                }


                window.draw(cell);
            }
        }


        // ==============================
        // TEXTO DE INTERFAZ
        // ==============================

        window.draw(controlsText);
        window.draw(statusText);


        window.display();
    }


    return 0;
}