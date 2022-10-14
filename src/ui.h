#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>

#include "ui_constants.h"
#include "ui_cell.h"
#include "pause_play.h"
#include "cell.h"

const unsigned int FONT_SIZE = 32;
const float MILLIS_BETWEEN_UPDATES = 10;

class UI {
  private:
    sf::Vector2u window_size;

    sf::Text title;
    void set_title_position();

    sf::Rect<unsigned int> game_view_box;
    std::vector<std::vector<UICell>> cells;
    sf::Vector2u ui_grid_pos;
    unsigned int cell_margin;
    void update_cell_positions();
    
    void update_ui_positioning();

    PausePlayButton pause_play_button;

    clock_t last_update;
  public:
    UI(const sf::Font &font, sf::Vector2u);

    void event_handler(sf::Event);

    void set_cell_locations(const std::list<Cell> &cells);
    const std::list<Cell> get_cell_locations() const;
    void update();
    void render(sf::RenderWindow &);

    const bool is_paused() const { return pause_play_button.get_is_paused(); }

    sf::Rect<unsigned int> get_game_view_box() { return game_view_box; }
};

static sf::Text& center_sfml_text_bounds(sf::Text &text) {
  sf::FloatRect textRect = text.getLocalBounds();

  text.setOrigin(
      textRect.left + textRect.width/2.0f,
      textRect.top + textRect.height/2.0f
      );

  return text;
}
