#pragma once
#include <SFML/System/Vector2.hpp>
#include <list>

class Cell {
  public:
    unsigned int x;
    unsigned int y;

    bool operator==(const Cell &other) const {
      return x == other.x && y == other.y;
    }

    Cell(unsigned int x, unsigned int y): x(x), y(y) {}
    Cell(const sf::Vector2u coord): x(coord.x), y(coord.y) {} 
    Cell(const Cell &cell): x(cell.x), y(cell.y) {}

    const std::list<Cell> surrounding_cells() const {
      std::list<Cell> surrounding_cells;

      surrounding_cells.push_back(Cell(x - 1, y - 1));
      surrounding_cells.push_back(Cell(x, y - 1));
      surrounding_cells.push_back(Cell(x + 1, y - 1));
      surrounding_cells.push_back(Cell(x - 1, y));
      surrounding_cells.push_back(Cell(x + 1, y));
      surrounding_cells.push_back(Cell(x - 1, y + 1));
      surrounding_cells.push_back(Cell(x, y + 1));
      surrounding_cells.push_back(Cell(x + 1, y + 1));

      return surrounding_cells;
    }
};

namespace std {
  template <>
    struct hash<Cell> {
      std::uint64_t operator()(const Cell& cell) const {
        std::uint64_t a = cell.x;
        return a << 32 | cell.y;
      }
    };
}
