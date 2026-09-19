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

    bool operator==(const Position &other) const
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
        const Position &current,
        const Position &goal);

    void begin(
        std::vector<std::vector<CellState>> &grid,
        const Position &start,
        const Position &goal);

    void step(
        std::vector<std::vector<CellState>> &grid);

    bool isRunning() const;
    bool pathFound() const;
    bool hasFinished() const;

    void reset();

private:
    std::vector<std::vector<Node>> nodes;
    std::vector<Position> openList;

    Position startPosition{-1, -1};
    Position goalPosition{-1, -1};

    bool running = false;
    bool found = false;
    bool finished = false;

    void reconstructPath(
        std::vector<std::vector<CellState>> &grid);
};