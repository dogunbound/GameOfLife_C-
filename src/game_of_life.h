#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <time.h>

#include "cell.h"

class GameOfLife {
  private:
    std::unordered_set<Cell> cells;
    bool paused;
    unsigned int millis_between_updates;
    clock_t last_update;

    void update_one_tick();
    void add_cell(const Cell &cell);
    void remove_cell(const Cell &cell);
  public:
    GameOfLife();

    void update();
    void only_active_cells_in_area(const std::list<Cell> &cells, const sf::Rect<unsigned int> & area);

    void set_paused(const bool &paused) { this->paused = paused; }
    const bool &get_paused() const { return paused; }

    const std::list<Cell> get_cells_in_area(const sf::Rect<unsigned int>&) const;
};
