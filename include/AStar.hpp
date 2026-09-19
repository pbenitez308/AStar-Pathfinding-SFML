#pragma once

#include <vector>
#include <limits>

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

struct Position
{
    int row;
    int col;

    bool operator==(const Position& other) const
    {
        return row == other.row &&
               col == other.col;
    }
};

struct Node
{
    Position position{-1, -1};

    int g = std::numeric_limits<int>::max();
    int h = 0;
    int f = std::numeric_limits<int>::max();

    Position parent{-1, -1};

    bool hasParent = false;
    bool inOpen = false;
    bool closed = false;
};

class AStar
{
public:

    static int heuristic(
        const Position& current,
        const Position& goal
    );

    static bool findPath(
        std::vector<std::vector<CellState>>& grid,
        const Position& start,
        const Position& goal
    );
};