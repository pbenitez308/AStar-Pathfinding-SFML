#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

#include "AStar.hpp"

// ============================================================
// CONFIGURACION
// ============================================================

const int WINDOW_WIDTH = 800;

const int GRID_HEIGHT = 600;
const int UI_HEIGHT = 90;

const int WINDOW_HEIGHT = GRID_HEIGHT + UI_HEIGHT;

const int CELL_SIZE = 40;

const int COLS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = GRID_HEIGHT / CELL_SIZE;


// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // VENTANA
    // ========================================================

    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "A* PathFinding"
    );


    // ========================================================
    // FUENTE
    // ========================================================

    sf::Font font("assets/Roboto-Regular.ttf");


    // ========================================================
    // TITULO
    // ========================================================

    sf::Text titleText(
        font,
        "A* PATHFINDING VISUALIZER",
        20
    );

    titleText.setPosition({16.f, 12.f});

    titleText.setFillColor(
        sf::Color(235, 238, 245)
    );


    // ========================================================
    // CONTROLES
    // ========================================================

    sf::Text controlsText(
        font,
        "LMB Start  |  RMB Goal  |  MMB Walls  |  SPACE Run  |  C Clear  |  R Reset",
        13
    );

    controlsText.setPosition({16.f, 57.f});

    controlsText.setFillColor(
        sf::Color(165, 170, 185)
    );


    // ========================================================
    // BARRA SUPERIOR
    // ========================================================

    sf::RectangleShape uiBar(
        sf::Vector2f(
            static_cast<float>(WINDOW_WIDTH),
            static_cast<float>(UI_HEIGHT)
        )
    );

    uiBar.setPosition({0.f, 0.f});

    uiBar.setFillColor(
        sf::Color(20, 20, 20)
    );


    // ========================================================
    // SEPARADOR
    // ========================================================

    sf::RectangleShape separator(
        sf::Vector2f(
            static_cast<float>(WINDOW_WIDTH),
            2.f
        )
    );

    separator.setPosition({
        0.f,
        static_cast<float>(UI_HEIGHT - 2)
    });

    separator.setFillColor(
        sf::Color(55, 60, 72)
    );


    // ========================================================
    // STATUS
    // ========================================================

    sf::RectangleShape statusBox(
        sf::Vector2f(165.f, 34.f)
    );

    statusBox.setPosition({620.f, 11.f});

    statusBox.setFillColor(
        sf::Color(35, 39, 48)
    );


    sf::Text statusText(
        font,
        "STATUS: READY",
        14
    );

    statusText.setPosition({637.f, 19.f});

    statusText.setFillColor(
        sf::Color(200, 205, 215)
    );


    // ========================================================
    // GRID
    // ========================================================

    std::vector<std::vector<CellState>> grid(
        ROWS,
        std::vector<CellState>(
            COLS,
            CellState::Empty
        )
    );


    // ========================================================
    // START / GOAL
    // ========================================================

    bool startPlaced = false;
    bool goalPlaced = false;

    Position startPosition{-1, -1};
    Position goalPosition{-1, -1};


    // ========================================================
    // MUROS
    // ========================================================

    bool drawingWalls = false;
    bool erasingWalls = false;


    // ========================================================
    // A*
    // ========================================================

    AStar astar;

    sf::Clock stepClock;


    // ========================================================
    // LOOP PRINCIPAL
    // ========================================================

    while (window.isOpen())
    {
        // ====================================================
        // EVENTOS
        // ====================================================

        while (const std::optional event = window.pollEvent())
        {
            // ------------------------------------------------
            // CERRAR
            // ------------------------------------------------

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }


            // =================================================
            // TECLADO
            // =================================================

            if (const auto* keyEvent =
                    event->getIf<sf::Event::KeyPressed>())
            {
                // ---------------------------------------------
                // R -> RESET COMPLETO
                // ---------------------------------------------

                if (keyEvent->code == sf::Keyboard::Key::R)
                {
                    for (int row = 0; row < ROWS; row++)
                    {
                        for (int col = 0; col < COLS; col++)
                        {
                            grid[row][col] =
                                CellState::Empty;
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


                // ---------------------------------------------
                // SPACE -> EJECUTAR A*
                // ---------------------------------------------

                if (keyEvent->code ==
                    sf::Keyboard::Key::Space)
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


                // ---------------------------------------------
                // C -> LIMPIAR BUSQUEDA
                // ---------------------------------------------

                if (keyEvent->code ==
                    sf::Keyboard::Key::C)
                {
                    for (int row = 0; row < ROWS; row++)
                    {
                        for (int col = 0; col < COLS; col++)
                        {
                            if (
                                grid[row][col] ==
                                    CellState::Open ||
                                grid[row][col] ==
                                    CellState::Closed ||
                                grid[row][col] ==
                                    CellState::Path
                            )
                            {
                                grid[row][col] =
                                    CellState::Empty;
                            }
                        }
                    }

                    astar.reset();
                }
            }


            // =================================================
            // CLICK DEL MOUSE
            // =================================================

            if (const auto* mouseEvent =
                    event->getIf<
                        sf::Event::MouseButtonPressed>())
            {
                // No editar mientras A* esta ejecutandose
                if (
                    !astar.isRunning() &&
                    mouseEvent->position.y >= UI_HEIGHT
                )
                {
                    int col =
                        mouseEvent->position.x /
                        CELL_SIZE;

                    int row =
                        (
                            mouseEvent->position.y -
                            UI_HEIGHT
                        ) /
                        CELL_SIZE;


                    // -----------------------------------------
                    // VERIFICAR LIMITES
                    // -----------------------------------------

                    if (
                        row >= 0 &&
                        row < ROWS &&
                        col >= 0 &&
                        col < COLS
                    )
                    {
                        // =====================================
                        // LMB -> START
                        // =====================================

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


                        // =====================================
                        // RMB -> GOAL
                        // =====================================

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


                        // =====================================
                        // MMB -> MUROS
                        // =====================================

                        if (
                            mouseEvent->button ==
                            sf::Mouse::Button::Middle
                        )
                        {
                            // Si inicia sobre muro:
                            // borrar
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

                            // Si inicia sobre vacio:
                            // dibujar
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

                            // No alterar Start, Goal,
                            // Path, Open o Closed
                            else
                            {
                                drawingWalls = false;
                                erasingWalls = false;
                            }
                        }
                    }
                }
            }


            // =================================================
            // SOLTAR BOTON CENTRAL
            // =================================================

            if (const auto* mouseEvent =
                    event->getIf<
                        sf::Event::MouseButtonReleased>())
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


            // =================================================
            // ARRASTRAR PARA CREAR / BORRAR MUROS
            // =================================================

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
                        (
                            moveEvent->position.y -
                            UI_HEIGHT
                        ) /
                        CELL_SIZE;


                    if (
                        row >= 0 &&
                        row < ROWS &&
                        col >= 0 &&
                        col < COLS
                    )
                    {
                        // -------------------------------------
                        // BORRAR MUROS
                        // -------------------------------------

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

                        // -------------------------------------
                        // CREAR MUROS
                        // -------------------------------------

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


        // ====================================================
        // EJECUTAR A* PASO A PASO
        // ====================================================

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


        // ====================================================
        // ACTUALIZAR STATUS
        // ====================================================

        if (astar.isRunning())
        {
            statusText.setString(
                "STATUS: SEARCHING..."
            );

            statusText.setFillColor(
                sf::Color(66, 165, 245)
            );

            statusBox.setFillColor(
                sf::Color(25, 55, 85)
            );
        }

        else if (astar.pathFound())
        {
            statusText.setString(
                "STATUS: PATH FOUND"
            );

            statusText.setFillColor(
                sf::Color(72, 220, 140)
            );

            statusBox.setFillColor(
                sf::Color(25, 70, 52)
            );
        }

        else if (astar.hasFinished())
        {
            statusText.setString(
                "STATUS: NO SOLUTION"
            );

            statusText.setFillColor(
                sf::Color(255, 100, 100)
            );

            statusBox.setFillColor(
                sf::Color(80, 35, 40)
            );
        }

        else
        {
            statusText.setString(
                "STATUS: READY"
            );

            statusText.setFillColor(
                sf::Color(200, 205, 215)
            );

            statusBox.setFillColor(
                sf::Color(35, 39, 48)
            );
        }


        // ====================================================
        // RENDER
        // ====================================================

        window.clear(
            sf::Color(18, 20, 25)
        );


        // ----------------------------------------------------
        // BARRA DE INTERFAZ
        // ----------------------------------------------------

        window.draw(uiBar);
        window.draw(separator);


        // ====================================================
        // CUADRICULA
        // ====================================================

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                sf::RectangleShape cell(
                    sf::Vector2f(
                        static_cast<float>(
                            CELL_SIZE - 2
                        ),
                        static_cast<float>(
                            CELL_SIZE - 2
                        )
                    )
                );


                // Posicion incluyendo el offset
                // de la barra superior
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


                // =============================================
                // COLORES
                // =============================================

                switch (grid[row][col])
                {
                    case CellState::Empty:
                    {
                        cell.setFillColor(
                            sf::Color(37, 41, 49)
                        );

                        break;
                    }

                    case CellState::Start:
                    {
                        cell.setFillColor(
                            sf::Color(46, 204, 113)
                        );

                        break;
                    }

                    case CellState::Goal:
                    {
                        cell.setFillColor(
                            sf::Color(231, 76, 90)
                        );

                        break;
                    }

                    case CellState::Wall:
                    {
                        cell.setFillColor(
                            sf::Color(12, 15, 20)
                        );

                        break;
                    }

                    case CellState::Open:
                    {
                        cell.setFillColor(
                            sf::Color(41, 182, 246)
                        );

                        break;
                    }

                    case CellState::Closed:
                    {
                        cell.setFillColor(
                            sf::Color(92, 107, 192)
                        );

                        break;
                    }

                    case CellState::Path:
                    {
                        cell.setFillColor(
                            sf::Color(255, 202, 40)
                        );

                        break;
                    }
                }


                window.draw(cell);
            }
        }


        // ====================================================
        // INTERFAZ
        // ====================================================

        window.draw(titleText);
        window.draw(controlsText);

        window.draw(statusBox);
        window.draw(statusText);


        // ====================================================
        // MOSTRAR FRAME
        // ====================================================

        window.display();
    }


    return 0;
}