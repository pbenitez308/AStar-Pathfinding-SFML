#pragma once

#include <vector>

// Estado visual/lógico de cada celda
enum class CellState
{
    Empty,
    Start,
    Goal,
    Wall,
    Open,
    Closed,
    Path
};

// Posición dentro de la matriz
struct Position
{
    int row;
    int col;

    bool operator==(const Position& other) const
    {
        return row == other.row && col == other.col;
    }
};

// Información utilizada por A*
struct Node
{
    Position position;

    int g;  // Costo desde Start
    int h;  // Heurística hasta Goal
    int f;  // g + h

    Position parent;
    bool hasParent;

    Node()
        : position{-1, -1},
          g(0),
          h(0),
          f(0),
          parent{-1, -1},
          hasParent(false)
    {
    }
};

class AStar
{
public:
    static int heuristic(
        const Position& current,
        const Position& goal
    );
};