#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <unordered_set>
#include <list>

#include "cell.h"

class ComputationBox {
  private:
    std::unordered_set<Cell> cells;
    sf::Rect<unsigned int> bounds;

    void compute_bounds();

    const bool is_cell_alive(const Cell &) const;
  public:
    ComputationBox(std::unordered_set<Cell> &);

    void merge(ComputationBox &);
    std::list<ComputationBox> split();
    void update_cells_one_tick();

    const bool intersects(const ComputationBox &) const;
    const bool is_empty() const;
    bool needs_to_split();

    const sf::Rect<unsigned int> get_bounds() const;

    const std::list<Cell> get_cells() const; 
};
