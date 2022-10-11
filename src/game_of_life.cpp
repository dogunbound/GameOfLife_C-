#include "game_of_life.h"
#include "computation_box.h"
#include <ctime>

void GameOfLife::split() {
  for (auto iter = computation_boxes.begin(); iter != computation_boxes.end(); iter++) {
    if (iter->needs_to_split()) {
      auto new_set_of_computation_boxes = iter->split();
      computation_boxes.erase(iter);
      computation_boxes.splice(computation_boxes.end(), new_set_of_computation_boxes);
    }
  }
}

void GameOfLife::merge() {
  bool merge_performed;
  do {
    merge_performed = false;
      for (auto iter = computation_boxes.begin(); iter != computation_boxes.end(); iter++) {
      for (auto &cbox : computation_boxes) {
        if (cbox.intersects(*iter)) {
          cbox.merge(*iter);
          computation_boxes.erase(iter);
          merge_performed = true;
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
  time_since_last_update = clock();
  paused = false;

  millis_between_updates = 500;
}

void GameOfLife::update() {
  if (((float) time_since_last_update) / CLOCKS_PER_SEC * 1000 > millis_between_updates && !paused) {
    update_computation_boxes_one_tick();
  }
}

const std::list<Cell> GameOfLife::get_active_cells_in_area(const sf::Rect<unsigned int> &area) const {
  std::list<Cell> active_cells_in_area;

  for (auto cbox : computation_boxes) {
    if (cbox.get_bounds().intersects(area))
      active_cells_in_area.splice(active_cells_in_area.end(), cbox.get_cells());
  }

  return active_cells_in_area;
}

void GameOfLife::activate_cell(const Cell &cell) {
  std::unordered_set<Cell> tmp_cells( { cell } );
  computation_boxes.push_back(ComputationBox(tmp_cells));

  split();
  merge();
}
