#include "game_of_life.h"
#include "computation_box.h"
#include <SFML/System/Vector2.hpp>
#include <ctime>

void GameOfLife::split() {
  for (auto iter = computation_boxes.begin(); iter != computation_boxes.end(); iter++) {
    if (iter->needs_to_split()) {
      auto new_set_of_computation_boxes = iter->split();
      computation_boxes.erase(iter);
      iter++;
      computation_boxes.splice(computation_boxes.end(), new_set_of_computation_boxes);

      if (iter == computation_boxes.end()) return;
    }
  }
}

void GameOfLife::merge() {
  bool merge_performed;
  do {
    merge_performed = false;
      for (auto iter = computation_boxes.begin(); iter != computation_boxes.end(); iter++) {
      for (auto &cbox : computation_boxes) {
        if (cbox.intersects(*iter) && &*iter != &cbox) {
          cbox.merge(*iter);
          computation_boxes.erase(iter);
          iter++;
          merge_performed = true;

          if (iter == computation_boxes.end()) return;
        }
      }
    }
  } while(merge_performed);
}

void GameOfLife::update_computation_boxes_one_tick() {
  for (auto &cbox : computation_boxes) {
    cbox.update_cells_one_tick();
  }

  split();
  merge();
}

GameOfLife::GameOfLife() {
  last_update = clock();
  paused = true;

  millis_between_updates = 50;
}

void GameOfLife::update() {
  if (clock() - last_update > millis_between_updates && !paused) {
    update_computation_boxes_one_tick();
    last_update = clock();
  }
}

const std::list<Cell> GameOfLife::get_active_cells_in_area(const sf::Rect<unsigned int> &area) const {
  std::list<Cell> active_cells_in_area;
  for (auto cbox : computation_boxes) {
    if (cbox.get_bounds().intersects(area)) {
      const auto cells = cbox.get_cells();
      active_cells_in_area.insert(active_cells_in_area.end(), cells.begin(), cells.end());
    }
  }

  return active_cells_in_area;
}

void GameOfLife::activate_cell(const Cell &cell) {
  for (const auto &cbox: computation_boxes) {
    if (cbox.get_bounds().contains(sf::Vector2u(
            cell.x,
            cell.y
            ))) {
      for (const auto &cbox_cell: cbox.get_cells()) {
        if (cbox_cell == cell)
          return;
      }
    }
  }
  std::unordered_set<Cell> tmp_cells( { cell } );
  computation_boxes.push_back(ComputationBox(tmp_cells));

  split();
  merge();
}

void GameOfLife::activate_cells_in_list(const std::list<Cell> &cells) {
  for (const auto &cell : cells) {
    this->activate_cell(cell);
  }
}
