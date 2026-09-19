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