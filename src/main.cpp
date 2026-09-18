#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int CELL_SIZE = 40;

const int COLS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = WINDOW_HEIGHT / CELL_SIZE;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "A* PathFinding"
    );

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
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

                cell.setFillColor(sf::Color(50, 50, 50));

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}