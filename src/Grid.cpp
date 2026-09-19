#include "Grid.hpp"


// ============================================================
// CONSTRUCTOR
// ============================================================

Grid::Grid(int topOffset)
    : topOffset(topOffset),

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
}


// ============================================================
// CONVERTIR MOUSE A COORDENADAS DEL GRID
// ============================================================

bool Grid::mouseToGrid(
    int mouseX,
    int mouseY,
    int& row,
    int& col
) const
{
    // El mouse esta sobre la interfaz superior
    if (mouseY < topOffset)
    {
        return false;
    }


    col =
        mouseX /
        CELL_SIZE;


    row =
        (mouseY - topOffset) /
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


// ============================================================
// MOUSE PRESIONADO
// ============================================================

void Grid::handleMousePressed(
    const sf::Event::MouseButtonPressed& event
)
{
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
    // CLICK IZQUIERDO -> START
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
    // CLICK DERECHO -> GOAL
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
    // CLICK CENTRAL -> MUROS
    // ========================================================

    if (
        event.button ==
        sf::Mouse::Button::Middle
    )
    {
        // Si empieza sobre un muro:
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

        // Si empieza sobre espacio vacio:
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

        // Start, Goal, Open, Closed y Path
        // no pueden modificarse como muros
        else
        {
            drawingWalls = false;

            erasingWalls = false;
        }
    }
}


// ============================================================
// MOUSE LIBERADO
// ============================================================

void Grid::handleMouseReleased(
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


// ============================================================
// MOUSE EN MOVIMIENTO
// ============================================================

void Grid::handleMouseMoved(
    const sf::Event::MouseMoved& event
)
{
    if (!drawingWalls)
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
    // BORRAR MUROS
    // ========================================================

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


        return;
    }


    // ========================================================
    // CREAR MUROS
    // ========================================================

    if (
        grid[row][col] ==
        CellState::Empty
    )
    {
        grid[row][col] =
            CellState::Wall;
    }
}


// ============================================================
// RESET COMPLETO
// ============================================================

void Grid::reset()
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


    startPosition =
        {-1, -1};

    goalPosition =
        {-1, -1};


    drawingWalls = false;
    erasingWalls = false;
}


// ============================================================
// LIMPIAR RESULTADOS DE A*
// ============================================================

void Grid::clearSearch()
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
}


// ============================================================
// DIBUJAR GRID
// ============================================================

void Grid::draw(
    sf::RenderWindow& window
) const
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
                        topOffset
                    )
                )
            );


            // =================================================
            // COLORES
            // =================================================

            switch (grid[row][col])
            {
                case CellState::Empty:
                {
                    cell.setFillColor(
                        sf::Color(
                            37,
                            41,
                            49
                        )
                    );

                    break;
                }


                case CellState::Start:
                {
                    cell.setFillColor(
                        sf::Color(
                            46,
                            204,
                            113
                        )
                    );

                    break;
                }


                case CellState::Goal:
                {
                    cell.setFillColor(
                        sf::Color(
                            231,
                            76,
                            90
                        )
                    );

                    break;
                }


                case CellState::Wall:
                {
                    cell.setFillColor(
                        sf::Color(
                            12,
                            15,
                            20
                        )
                    );

                    break;
                }


                case CellState::Open:
                {
                    cell.setFillColor(
                        sf::Color(
                            41,
                            182,
                            246
                        )
                    );

                    break;
                }


                case CellState::Closed:
                {
                    cell.setFillColor(
                        sf::Color(
                            92,
                            107,
                            192
                        )
                    );

                    break;
                }


                case CellState::Path:
                {
                    cell.setFillColor(
                        sf::Color(
                            255,
                            202,
                            40
                        )
                    );

                    break;
                }
            }


            window.draw(cell);
        }
    }
}


// ============================================================
// START / GOAL
// ============================================================

bool Grid::hasStart() const
{
    return startPlaced;
}


bool Grid::hasGoal() const
{
    return goalPlaced;
}


Position Grid::getStartPosition() const
{
    return startPosition;
}


Position Grid::getGoalPosition() const
{
    return goalPosition;
}


// ============================================================
// ACCESO A CELDAS
// ============================================================

std::vector<std::vector<CellState>>& Grid::cells()
{
    return grid;
}


const std::vector<std::vector<CellState>>&
Grid::cells() const
{
    return grid;
}