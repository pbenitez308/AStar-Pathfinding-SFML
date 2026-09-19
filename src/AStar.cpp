#include "AStar.hpp"

#include <cstdlib>
#include <vector>


int AStar::heuristic(
    const Position& current,
    const Position& goal
)
{
    return std::abs(current.row - goal.row) +
           std::abs(current.col - goal.col);
}


bool AStar::findPath(
    std::vector<std::vector<CellState>>& grid,
    const Position& start,
    const Position& goal
)
{
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());


    // Limpiar resultados anteriores de A*
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


    // Crear matriz de nodos
    std::vector<std::vector<Node>> nodes(
        rows,
        std::vector<Node>(cols)
    );


    // Guardar posición de cada nodo
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            nodes[row][col].position = {row, col};
        }
    }


    // Configurar nodo inicial
    Node& startNode = nodes[start.row][start.col];

    startNode.g = 0;
    startNode.h = heuristic(start, goal);
    startNode.f = startNode.g + startNode.h;


    // Lista de nodos pendientes de explorar
    std::vector<Position> openList;

    openList.push_back(start);
    startNode.inOpen = true;


    // Movimiento en 4 direcciones
    const int directions[4][2] =
    {
        {-1, 0},   // arriba
        { 1, 0},   // abajo
        { 0,-1},   // izquierda
        { 0, 1}    // derecha
    };


    while (!openList.empty())
    {
        // Buscar nodo con menor f
        int bestIndex = 0;

        for (int i = 1;
             i < static_cast<int>(openList.size());
             i++)
        {
            Position candidatePos = openList[i];
            Position bestPos = openList[bestIndex];

            Node& candidate =
                nodes[candidatePos.row][candidatePos.col];

            Node& best =
                nodes[bestPos.row][bestPos.col];


            // Menor f es mejor.
            // Si empatan, menor h.
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


        // ¿Llegamos al objetivo?
        if (current == goal)
        {
            Position currentPath = goal;


            // Reconstruir camino hacia Start
            while (!(currentPath == start))
            {
                Node& pathNode =
                    nodes[currentPath.row][currentPath.col];

                if (!pathNode.hasParent)
                {
                    return false;
                }

                currentPath = pathNode.parent;


                if (!(currentPath == start) &&
                    !(currentPath == goal))
                {
                    grid[currentPath.row][currentPath.col]
                        = CellState::Path;
                }
            }

            return true;
        }


        // Marcar como cerrado
        if (!(current == start) &&
            !(current == goal))
        {
            grid[current.row][current.col]
                = CellState::Closed;
        }


        // Revisar vecinos
        for (const auto& direction : directions)
        {
            Position neighbor =
            {
                current.row + direction[0],
                current.col + direction[1]
            };


            // Límites de la matriz
            if (neighbor.row < 0 ||
                neighbor.row >= rows ||
                neighbor.col < 0 ||
                neighbor.col >= cols)
            {
                continue;
            }


            // Ignorar muros
            if (grid[neighbor.row][neighbor.col]
                == CellState::Wall)
            {
                continue;
            }


            Node& neighborNode =
                nodes[neighbor.row][neighbor.col];


            // Ignorar nodos ya cerrados
            if (neighborNode.closed)
            {
                continue;
            }


            int tentativeG =
                currentNode.g + 1;


            // ¿Encontramos una ruta mejor?
            if (tentativeG < neighborNode.g)
            {
                neighborNode.parent = current;
                neighborNode.hasParent = true;

                neighborNode.g = tentativeG;

                neighborNode.h =
                    heuristic(neighbor, goal);

                neighborNode.f =
                    neighborNode.g +
                    neighborNode.h;


                // Agregar a Open List
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


    // No existe camino
    return false;
}