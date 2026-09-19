#include "AStar.hpp"

#include <cstdlib>


int AStar::heuristic(
    const Position& current,
    const Position& goal
)
{
    return std::abs(current.row - goal.row) +
           std::abs(current.col - goal.col);
}


void AStar::begin(
    std::vector<std::vector<CellState>>& grid,
    const Position& start,
    const Position& goal
)
{
    reset();

    startPosition = start;
    goalPosition = goal;

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Limpiar resultados anteriores
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (grid[row][col] == CellState::Open ||
                grid[row][col] == CellState::Closed ||
                grid[row][col] == CellState::Path)
            {
                grid[row][col] = CellState::Empty;
            }
        }
    }

    // Crear nodos
    nodes = std::vector<std::vector<Node>>(
        rows,
        std::vector<Node>(cols)
    );

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            nodes[row][col].position = {row, col};
        }
    }

    Node& startNode =
        nodes[start.row][start.col];

    startNode.g = 0;
    startNode.h = heuristic(start, goal);
    startNode.f = startNode.g + startNode.h;

    openList.push_back(start);
    startNode.inOpen = true;

    running = true;
    found = false;
}


void AStar::step(
    std::vector<std::vector<CellState>>& grid
)
{
    if (!running)
    {
        return;
    }

    // No existe camino
    if (openList.empty())
    {
        running = false;
        found = false;
        return;
    }

    // Buscar nodo con menor f
    int bestIndex = 0;

    for (int i = 1;
         i < static_cast<int>(openList.size());
         i++)
    {
        Position candidatePosition = openList[i];
        Position bestPosition = openList[bestIndex];

        Node& candidate =
            nodes[candidatePosition.row][candidatePosition.col];

        Node& best =
            nodes[bestPosition.row][bestPosition.col];

        if (candidate.f < best.f ||
            (candidate.f == best.f &&
             candidate.h < best.h))
        {
            bestIndex = i;
        }
    }

    Position current = openList[bestIndex];

    openList.erase(
        openList.begin() + bestIndex
    );

    Node& currentNode =
        nodes[current.row][current.col];

    currentNode.inOpen = false;
    currentNode.closed = true;

    // Llegamos al objetivo
    if (current == goalPosition)
    {
        found = true;
        running = false;

        reconstructPath(grid);

        return;
    }

    // Marcar nodo procesado
    if (!(current == startPosition))
    {
        grid[current.row][current.col]
            = CellState::Closed;
    }

    const int directions[4][2] =
    {
        {-1, 0},
        { 1, 0},
        { 0,-1},
        { 0, 1}
    };

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Explorar vecinos
    for (const auto& direction : directions)
    {
        Position neighbor =
        {
            current.row + direction[0],
            current.col + direction[1]
        };

        // Fuera del mapa
        if (neighbor.row < 0 ||
            neighbor.row >= rows ||
            neighbor.col < 0 ||
            neighbor.col >= cols)
        {
            continue;
        }

        // Muro
        if (grid[neighbor.row][neighbor.col]
            == CellState::Wall)
        {
            continue;
        }

        Node& neighborNode =
            nodes[neighbor.row][neighbor.col];

        if (neighborNode.closed)
        {
            continue;
        }

        int tentativeG =
            currentNode.g + 1;

        if (tentativeG < neighborNode.g)
        {
            neighborNode.parent = current;
            neighborNode.hasParent = true;

            neighborNode.g = tentativeG;
            neighborNode.h =
                heuristic(neighbor, goalPosition);

            neighborNode.f =
                neighborNode.g +
                neighborNode.h;

            if (!neighborNode.inOpen)
            {
                openList.push_back(neighbor);
                neighborNode.inOpen = true;

                if (grid[neighbor.row][neighbor.col]
                    == CellState::Empty)
                {
                    grid[neighbor.row][neighbor.col]
                        = CellState::Open;
                }
            }
        }
    }
}


void AStar::reconstructPath(
    std::vector<std::vector<CellState>>& grid
)
{
    Position current = goalPosition;

    while (!(current == startPosition))
    {
        Node& node =
            nodes[current.row][current.col];

        if (!node.hasParent)
        {
            return;
        }

        current = node.parent;

        if (!(current == startPosition) &&
            !(current == goalPosition))
        {
            grid[current.row][current.col]
                = CellState::Path;
        }
    }
}


bool AStar::isRunning() const
{
    return running;
}


bool AStar::pathFound() const
{
    return found;
}


void AStar::reset()
{
    nodes.clear();
    openList.clear();

    startPosition = {-1, -1};
    goalPosition = {-1, -1};

    running = false;
    found = false;
}