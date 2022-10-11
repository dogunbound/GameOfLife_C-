#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#include "ui_constants.h"
#include "ui_cell.h"

const unsigned int FONT_SIZE = 32;

class UI {
  private:
    sf::Vector2u window_size;

    sf::Text title;
    void set_title_position();

    std::vector<std::vector<UICell>> cells;
    sf::Vector2u ui_grid_pos;
    unsigned int cell_margin;
    void update_cell_positions();
    
    void update_ui_positioning();
  public:
    UI(const sf::Font &font, sf::Vector2u);

    void event_handler(sf::Event);

    void update();
    void render(sf::RenderWindow &);
};

static sf::Text& center_sfml_text_bounds(sf::Text &text) {
  sf::FloatRect textRect = text.getLocalBounds();

  text.setOrigin(
      textRect.left + textRect.width/2.0f,
      textRect.top + textRect.height/2.0f
      );

  return text;
}
