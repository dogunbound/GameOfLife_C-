#include "computation_box.h"
#include <SFML/System/Vector2.hpp>
#include <limits>
#include <algorithm>
#include <unordered_set>

const unsigned int BOUNDS_OFFSET = 3;

ComputationBox::ComputationBox(std::unordered_set<Cell> &cells): cells(cells), bounds(sf::Rect<unsigned int>(0, 0, 0, 0)) {
  compute_bounds();
}

std::list<ComputationBox> ComputationBox::split() {
  std::list<ComputationBox> cboxes;

  for (const auto &cell : cells) {
    auto tmp_cells = std::unordered_set<Cell>( { cell } );
    cboxes.push_back(ComputationBox(tmp_cells));
  }

  bool merge_performed;
  do {
    merge_performed = false;

    for (auto &cbox : cboxes) {
      for (auto iter = cboxes.begin(); iter != cboxes.end(); iter++) {
        if (cbox.intersects(*iter) && &*iter != &cbox) {
          cbox.merge(*iter);
          cboxes.erase(iter);
          merge_performed = true;
        }
      }
    } 
  } while (merge_performed);

  return cboxes;
}

void ComputationBox::merge(ComputationBox &computation_box) {
  for (const auto &c : computation_box.cells) {
    cells.insert(c);
  }

  compute_bounds();
}

void ComputationBox::compute_bounds() {
  sf::Vector2u top_left_pos(std::numeric_limits<unsigned int>::max(), 0);
  sf::Vector2u bottom_right_pos(0, std::numeric_limits<unsigned int>::max());
  for (const auto &cell: cells) {
    if (cell.x < top_left_pos.x) {
      top_left_pos.x = cell.x;
    }

    if (cell.y > top_left_pos.y) {
      top_left_pos.y = cell.y;
    }

    if (cell.x > bottom_right_pos.x) {
      bottom_right_pos.x = cell.x;
    }

    if (cell.y < bottom_right_pos.y) {
      bottom_right_pos.y = cell.y;
    }
  }

  bounds = sf::Rect<unsigned int>(
      top_left_pos.x, 
      top_left_pos.y,
      bottom_right_pos.x - top_left_pos.x,
      top_left_pos.y - bottom_right_pos.y
      );
}

const sf::Rect<unsigned int> ComputationBox::get_bounds() const {
  return sf::Rect<unsigned int>(
      bounds.left - BOUNDS_OFFSET,
      bounds.top - BOUNDS_OFFSET,
      bounds.width + BOUNDS_OFFSET * 2,
      bounds.height + BOUNDS_OFFSET * 2
      );
}

const bool ComputationBox::intersects(const ComputationBox &computation_box) const { return this->get_bounds().intersects(computation_box.get_bounds()); }

const bool ComputationBox::is_empty() const {
  return cells.size() > 0;
}

void ComputationBox::update_cells_one_tick() {
  std::unordered_set<Cell> cells_to_compute;

  for (const auto &cell : cells) {
    std::list<Cell> surrounding_cells = cell.surrounding_cells();
    cells_to_compute.insert(surrounding_cells.begin(), surrounding_cells.end());
    cells_to_compute.insert(cell);
  }

  std::unordered_set<Cell> new_cells;
  for (const auto &cell : cells) {
    if (is_cell_alive(cell)) {
      new_cells.insert(cell);
    }
  }

  compute_bounds();
  cells = new_cells;
}

const bool ComputationBox::is_cell_alive(const Cell &cell) const {
  auto surrounding_cells = cell.surrounding_cells();

  unsigned short num_surrounding_cells = 0;
  for (const auto &cell : surrounding_cells) {
    if (cells.find(cell) != cells.end()) {
      num_surrounding_cells++;
    }
  }

  return num_surrounding_cells == 2 || num_surrounding_cells == 3;
}

bool ComputationBox::needs_to_split() {
  return split().size() > 1;
}

const std::list<Cell> ComputationBox::get_cells() const {
  return std::list<Cell>(cells.begin(), cells.end());
}
