#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <unordered_set>
#include <tuple>
#include <time.h>

#include "cell.h"
#include "helper.h"

class GameOfLife {
  private:
    std::unordered_set<Cell> active_cells;
    bool is_paused;
    unsigned int millis_between_updates;
    clock_t time_of_last_update;
    sf::Rect<unsigned int> game_view_box;
  public:
  private:
    const std::list<std::tuple<AddOrRemoveCell, Cell>> update_one_tick();
  public:
    GameOfLife(const sf::Rect<unsigned int> game_view_box);
    const std::list<std::tuple<AddOrRemoveCell, Cell>> update();

    void set_is_paused(const bool is_paused) { this->is_paused = is_paused; }
    const bool get_is_paused() const { return is_paused; }
    const std::list<std::tuple<AddOrRemoveCell, Cell>> set_game_view_box(const sf::Rect<unsigned int> game_view_box); 
    const sf::Rect<unsigned int> get_game_view_box() const { return game_view_box; }

    void set_cell_locations_in_game_view_box(const std::list<std::tuple<AddOrRemoveCell, Cell>> &changed_cells);
};
