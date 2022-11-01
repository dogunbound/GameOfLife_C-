#include "game_of_life.h"
#include <SFML/Graphics/Rect.hpp>
#include <limits.h>

GameOfLife::GameOfLife() : 
  cells(std::unordered_set<Cell>()),
  paused(true),
  millis_between_updates(250),
  last_update(clock()) {}

void GameOfLife::update_one_tick() {
  std::unordered_set<Cell> cells_to_check;

  for (const auto &cell : cells) {
    for (const auto &s_cells : cell.surrounding_cells()) {
      cells_to_check.insert(s_cells);
    }
    cells_to_check.insert(cell);
  }

  std::unordered_set<Cell> new_cell_locations;
  for (const auto &cell_to_check : cells_to_check) {
    unsigned short num_surrounding_cells = 0;
    for (const auto &s_cells : cell_to_check.surrounding_cells()) {
      if (cells.find(s_cells) != cells.end()) {
        num_surrounding_cells++;
      }
    }

    if (cells.find(cell_to_check) != cells.end() && (num_surrounding_cells == 2 || num_surrounding_cells == 3)) {
      new_cell_locations.insert(cell_to_check);
    } else if (num_surrounding_cells == 3) {
      new_cell_locations.insert(cell_to_check);
    }
  }

  cells = std::move(new_cell_locations);
}

void GameOfLife::update() {
  if (((float) clock() - (float) last_update) / (float) CLOCKS_PER_SEC * 1000.0f < millis_between_updates || paused)
    return;

  update_one_tick();
  last_update = clock();
}

const std::list<Cell> GameOfLife::get_cells_in_area(const sf::Rect<unsigned int>& area) const {
  std::list<Cell> cells_in_area;
  const auto x_max = area.left + area.width;
  const auto y_max = area.top + area.height;
  for (auto x = area.left; x < x_max; x++) {
    for (auto y = area.top; y < y_max; y++) {
      if (cells.find(Cell(x, y)) != cells.end()) {
        cells_in_area.push_back(Cell(x, y));
      }
    }
  }

  return cells_in_area;
}

void GameOfLife::add_cell(const Cell &cell) {
  cells.insert(cell);
}

void GameOfLife::only_active_cells_in_area(const std::list<Cell> &cells, const sf::Rect<unsigned int> & area) {
  for (auto x = area.left; x < area.left + area.width ; x++) {
    for (auto y = area.top; y < area.top + area.height ; y++) {
      this->cells.erase(Cell(x, y));
    }
  }

  for (const auto &cell : cells) {
    this->add_cell(cell);
  }
}
