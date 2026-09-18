#include <SFML/Graphics.hpp>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int CELL_SIZE = 40;

const int COLS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = WINDOW_HEIGHT / CELL_SIZE;

enum class CellState
{
    Empty,
    Start,
    Goal,
    Wall
};

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "A* PathFinding"
    );

    std::vector<std::vector<CellState>> grid(
        ROWS,
        std::vector<CellState>(COLS, CellState::Empty)
    );

    bool startPlaced = false;
    bool goalPlaced = false;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Cerrar ventana
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Detectar clic del mouse
            if (const auto* mouseEvent =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                int col = mouseEvent->position.x / CELL_SIZE;
                int row = mouseEvent->position.y / CELL_SIZE;

                // Verificar que estamos dentro de la cuadrícula
                if (row >= 0 && row < ROWS &&
                    col >= 0 && col < COLS)
                {
                    // Clic izquierdo -> Start
                    if (mouseEvent->button == sf::Mouse::Button::Left)
                    {
                        if (!startPlaced &&
                            grid[row][col] == CellState::Empty)
                        {
                            grid[row][col] = CellState::Start;
                            startPlaced = true;
                        }
                    }

                    // Clic derecho -> Goal
                    if (mouseEvent->button == sf::Mouse::Button::Right)
                    {
                        if (!goalPlaced &&
                            grid[row][col] == CellState::Empty)
                        {
                            grid[row][col] = CellState::Goal;
                            goalPlaced = true;
                        }
                    }
                }
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
                        static_cast<float>(CELL_SIZE - 1)
                    )
                );

                cell.setPosition(
                    sf::Vector2f(
                        static_cast<float>(col * CELL_SIZE),
                        static_cast<float>(row * CELL_SIZE)
                    )
                );

                // Color según el estado de la celda
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
                }

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}