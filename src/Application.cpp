#include "Application.hpp"


// ============================================================
// CONSTRUCTOR
// ============================================================

Application::Application()
    : window(
          sf::VideoMode({
              WINDOW_WIDTH,
              WINDOW_HEIGHT
          }),
          "A* PathFinding"
      ),

      font("assets/Roboto-Regular.ttf"),

      titleText(
          font,
          "A* PATHFINDING VISUALIZER",
          20
      ),

      controlsText(
          font,
          "Left Mouse Start | Right Mouse Goal | Middle Mouse Walls |  SPACE Run  |  C Clear  |  R Reset",
          13
      ),

      statusText(
          font,
          "STATUS: READY",
          14
      ),

      uiBar(
          sf::Vector2f(
              static_cast<float>(WINDOW_WIDTH),
              static_cast<float>(UI_HEIGHT)
          )
      ),

      separator(
          sf::Vector2f(
              static_cast<float>(WINDOW_WIDTH),
              2.f
          )
      ),

      statusBox(
          sf::Vector2f(
              165.f,
              34.f
          )
      ),

      grid(
          ROWS,
          std::vector<CellState>(
              COLS,
              CellState::Empty
          )
      ),

      startPlaced(false),
      goalPlaced(false),

      startPosition{-1, -1},
      goalPosition{-1, -1},

      drawingWalls(false),
      erasingWalls(false)
{
    // ========================================================
    // TITULO
    // ========================================================

    titleText.setPosition({16.f, 12.f});

    titleText.setFillColor(
        sf::Color(235, 238, 245)
    );


    // ========================================================
    // CONTROLES
    // ========================================================

    controlsText.setPosition({16.f, 57.f});

    controlsText.setFillColor(
        sf::Color(165, 170, 185)
    );


    // ========================================================
    // STATUS
    // ========================================================

    statusText.setPosition({637.f, 19.f});

    statusText.setFillColor(
        sf::Color(200, 205, 215)
    );


    statusBox.setPosition({620.f, 11.f});

    statusBox.setFillColor(
        sf::Color(35, 39, 48)
    );


    // ========================================================
    // UI BAR
    // ========================================================

    uiBar.setPosition({0.f, 0.f});

    uiBar.setFillColor(
        sf::Color(20, 20, 20)
    );


    // ========================================================
    // SEPARADOR
    // ========================================================

    separator.setPosition({
        0.f,
        static_cast<float>(UI_HEIGHT - 2)
    });

    separator.setFillColor(
        sf::Color(55, 60, 72)
    );
}

void Application::run()
{
    while (window.isOpen())
    {
        processEvents();

        update();

        render();
    }
}

void Application::processEvents()
{
    while (const std::optional event =
               window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }


        if (const auto* keyEvent =
                event->getIf<
                    sf::Event::KeyPressed>())
        {
            handleKeyboard(*keyEvent);
        }


        if (const auto* mouseEvent =
                event->getIf<
                    sf::Event::MouseButtonPressed>())
        {
            handleMousePressed(*mouseEvent);
        }


        if (const auto* mouseEvent =
                event->getIf<
                    sf::Event::MouseButtonReleased>())
        {
            handleMouseReleased(*mouseEvent);
        }


        if (const auto* moveEvent =
                event->getIf<
                    sf::Event::MouseMoved>())
        {
            handleMouseMoved(*moveEvent);
        }
    }
}

void Application::update()
{
    updateAStar();
    updateStatus();
}

void Application::handleKeyboard(
    const sf::Event::KeyPressed& event
)
{
    // R -> Reset completo
    if (event.code == sf::Keyboard::Key::R)
    {
        resetBoard();
    }


    // C -> Limpiar busqueda
    if (event.code == sf::Keyboard::Key::C)
    {
        clearSearch();
    }


    // SPACE -> Ejecutar A*
    if (event.code ==
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
}

bool Application::mouseToGrid(
    int mouseX,
    int mouseY,
    int& row,
    int& col
) const
{
    if (mouseY < UI_HEIGHT)
    {
        return false;
    }


    col = mouseX / CELL_SIZE;

    row =
        (mouseY - UI_HEIGHT) /
        CELL_SIZE;


    if (
        row < 0 ||
        row >= ROWS ||
        col < 0 ||
        col >= COLS
    )
    {
        return false;
    }


    return true;
}

void Application::handleMousePressed(
    const sf::Event::MouseButtonPressed& event
)
{
    if (astar.isRunning())
    {
        return;
    }


    int row;
    int col;


    if (!mouseToGrid(
            event.position.x,
            event.position.y,
            row,
            col))
    {
        return;
    }


    // ========================================================
    // START
    // ========================================================

    if (
        event.button ==
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

        return;
    }


    // ========================================================
    // GOAL
    // ========================================================

    if (
        event.button ==
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

        return;
    }


    // ========================================================
    // MUROS
    // ========================================================

    if (
        event.button ==
        sf::Mouse::Button::Middle
    )
    {
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

        else
        {
            drawingWalls = false;
            erasingWalls = false;
        }
    }
}

void Application::handleMouseReleased(
    const sf::Event::MouseButtonReleased& event
)
{
    if (
        event.button ==
        sf::Mouse::Button::Middle
    )
    {
        drawingWalls = false;
        erasingWalls = false;
    }
}

void Application::handleMouseMoved(
    const sf::Event::MouseMoved& event
)
{
    if (
        !drawingWalls ||
        astar.isRunning()
    )
    {
        return;
    }


    int row;
    int col;


    if (!mouseToGrid(
            event.position.x,
            event.position.y,
            row,
            col))
    {
        return;
    }


    // Borrar muros
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

    // Dibujar muros
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

void Application::updateAStar()
{
    if (!astar.isRunning())
    {
        return;
    }


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

void Application::updateStatus()
{
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

        return;
    }


    if (astar.pathFound())
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

        return;
    }


    if (astar.hasFinished())
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

        return;
    }


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

void Application::render()
{
    window.clear(
        sf::Color(18, 20, 25)
    );


    drawGrid();

    drawInterface();


    window.display();
}

void Application::drawInterface()
{
    window.draw(uiBar);

    window.draw(separator);

    window.draw(titleText);

    window.draw(controlsText);

    window.draw(statusBox);

    window.draw(statusText);
}

void Application::drawGrid()
{
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


            switch (grid[row][col])
            {
                case CellState::Empty:
                    cell.setFillColor(
                        sf::Color(37, 41, 49)
                    );
                    break;


                case CellState::Start:
                    cell.setFillColor(
                        sf::Color(46, 204, 113)
                    );
                    break;


                case CellState::Goal:
                    cell.setFillColor(
                        sf::Color(231, 76, 90)
                    );
                    break;


                case CellState::Wall:
                    cell.setFillColor(
                        sf::Color(12, 15, 20)
                    );
                    break;


                case CellState::Open:
                    cell.setFillColor(
                        sf::Color(41, 182, 246)
                    );
                    break;


                case CellState::Closed:
                    cell.setFillColor(
                        sf::Color(92, 107, 192)
                    );
                    break;


                case CellState::Path:
                    cell.setFillColor(
                        sf::Color(255, 202, 40)
                    );
                    break;
            }


            window.draw(cell);
        }
    }
}

void Application::resetBoard()
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

void Application::clearSearch()
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