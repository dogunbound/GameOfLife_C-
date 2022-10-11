#pragma once
#include <string>
#include <sstream>
#include <list>

class Cell {
  public:
    unsigned int x;
    unsigned int y;

    bool operator==(const Cell &other) const {
      return x == other.x && y == other.y;
    }

    Cell(unsigned int x, unsigned int y): x(x), y(y) {}

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
      std::size_t operator()(const Cell& cell) const {
        std::string str = std::to_string(cell.x + 10) + std::to_string(cell.y + 10);
        std::stringstream sstream(str);

        std::size_t seed;
        sstream >> seed;
        return seed;
      }
    };
}
