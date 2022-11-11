#include "game_of_life.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <limits.h>

GameOfLife::GameOfLife(const sf::Rect<unsigned int> game_view_box) {
  active_cells = std::unordered_set<Cell>();
  is_paused = true;
  millis_between_updates = 250;
  time_of_last_update = clock();
  this->game_view_box = game_view_box;
}

const std::list<std::tuple<AddOrRemoveCell, Cell>> GameOfLife::update() {
  if (((float) clock() - (float) time_of_last_update) / (float) CLOCKS_PER_SEC * 1000.0f < millis_between_updates || is_paused)
    return std::list<std::tuple<AddOrRemoveCell, Cell>>();

  time_of_last_update = clock();
  return update_one_tick();
}

const std::list<std::tuple<AddOrRemoveCell, Cell>> GameOfLife::update_one_tick() {
  std::unordered_set<Cell> cells_to_check;

  for (const auto &cell : active_cells) {
    for (const auto &s_cells : cell.surrounding_cells()) {
      cells_to_check.insert(s_cells);
    }
    cells_to_check.insert(cell);
  }

  std::unordered_set<Cell> new_cell_locations;
  std::list<std::tuple<AddOrRemoveCell, Cell>> changed_cells_in_game_view_box;
  for (const auto &cell_to_check : cells_to_check) {
    unsigned short num_surrounding_cells = 0;
    for (const auto &s_cells : cell_to_check.surrounding_cells()) {
      if (active_cells.find(s_cells) != active_cells.end()) {
        num_surrounding_cells++;
      }
    }

    bool did_insert_cell = false;
    if (active_cells.find(cell_to_check) != active_cells.end() && (num_surrounding_cells == 2 || num_surrounding_cells == 3)) {
      new_cell_locations.insert(cell_to_check);
      did_insert_cell = true;
    } else if (num_surrounding_cells == 3) {
      new_cell_locations.insert(cell_to_check);
      did_insert_cell = true;
    }

    if (did_insert_cell && game_view_box.contains(sf::Vector2u(cell_to_check.x, cell_to_check.y))) changed_cells_in_game_view_box.push_back(std::make_tuple(AddOrRemoveCell::Add, cell_to_check));
    else if (game_view_box.contains(sf::Vector2u(cell_to_check.x, cell_to_check.y))) changed_cells_in_game_view_box.push_back(std::make_tuple(AddOrRemoveCell::Remove, cell_to_check));
  }

  active_cells = std::move(new_cell_locations);
  return changed_cells_in_game_view_box;
}

void GameOfLife::set_cell_locations_in_game_view_box(const std::list<std::tuple<AddOrRemoveCell, Cell>> &changed_cells) {
  for (const auto &cc : changed_cells)
    if (std::get<0>(cc) == AddOrRemoveCell::Add) active_cells.insert(std::get<1>(cc));
    else active_cells.erase(std::get<1>(cc));
}

// UnOptimized version
// To optimize, ONLY look at the difference between the new and old game_view_boxes.
// Doing it the ugly way because I don't want to prematurely optimize
const std::list<std::tuple<AddOrRemoveCell, Cell>> GameOfLife::set_game_view_box(const sf::Rect<unsigned int> game_view_box) {
  std::unordered_set<Cell> cells_in_old_game_view_box;
  std::unordered_set<Cell> cells_in_new_game_view_box;

  for (auto x = this->game_view_box.left; x < this->game_view_box.left + this->game_view_box.width; x++)
    for (auto y = this->game_view_box.top; y < this->game_view_box.top + this->game_view_box.height; y++)
      if (active_cells.find(Cell(x, y)) != active_cells.end()) cells_in_old_game_view_box.insert(Cell(x, y));

  for (auto x = game_view_box.left; x < game_view_box.left + game_view_box.width; x++)
    for (auto y = game_view_box.top; y < game_view_box.top + game_view_box.height; y++)
      if (active_cells.find(Cell(x, y)) != active_cells.end()) cells_in_new_game_view_box.insert(Cell(x, y));

  std::list<std::tuple<AddOrRemoveCell, Cell>> changed_cells;
  for (const auto &cell : cells_in_old_game_view_box)
    if (cells_in_new_game_view_box.find(cell) == cells_in_new_game_view_box.end()) changed_cells.push_back(std::make_tuple(AddOrRemoveCell::Remove, cell));
  for (const auto &cell : cells_in_new_game_view_box)
    if (cells_in_old_game_view_box.find(cell) == cells_in_old_game_view_box.end()) changed_cells.push_back(std::make_tuple(AddOrRemoveCell::Add, cell));

  this->game_view_box = game_view_box;

  return changed_cells;
}

