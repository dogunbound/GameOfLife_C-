#pragma once
#include <list>
#include <time.h>

#include "computation_box.h"

class GameOfLife {
  private:
    std::list<ComputationBox> computation_boxes;
    clock_t last_update;
    unsigned int millis_between_updates;
    bool paused;

    void update_computation_boxes_one_tick();

    void split();
    void merge();
  public:
    void update();

    void activate_cells_in_list(const std::list<Cell> &cells);
    void set_paused(bool pause) { paused = pause; };
    void set_millis_between_updates(unsigned int millis) { millis_between_updates = millis; }; 
    const unsigned int get_millis_between_updates() const { return millis_between_updates; };
    const std::list<Cell> get_active_cells_in_area(const sf::Rect<unsigned int> &) const;

    void activate_cell(const Cell &);
    
    GameOfLife();
};
