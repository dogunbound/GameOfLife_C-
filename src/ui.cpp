#include "ui.h"
#include "ui_constants.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>


void UI::update_ui_positioning() {
  set_title_position();
  update_cell_positions();
}

void UI::set_title_position() {
  const float amt_push_downwards = title.getLocalBounds().height / 2 + 10;
  sf::Vector2f pos = sf::Vector2f(
      window_size.x / 2.f,
      amt_push_downwards
      );
  title.setPosition(pos);
}

void UI::update_cell_positions() {
  const sf::Vector2u grid_size(window_size.x * 96 / 100, window_size.y * 80 / 100);
  const sf::Vector2u center_of_screen(window_size.x / 2, window_size.y / 2);

  const UICell current_cell = cells[0][0];
  const float cell_size = current_cell.get_regular_cell_size();
  const unsigned int distance_between_each_print = cell_size + cell_margin;
  const sf::Vector2u num_of_cells_to_display(
      floor((float) grid_size.x / (float) distance_between_each_print),
      floor((float) grid_size.y / (float) distance_between_each_print)
      );

  const sf::Vector2u corrected_grid_size(
      num_of_cells_to_display.x * distance_between_each_print,
      num_of_cells_to_display.y * distance_between_each_print
      );

  ui_grid_pos = sf::Vector2u(
      center_of_screen.x - corrected_grid_size.x / 2,
      center_of_screen.y - corrected_grid_size.y / 2
      );

  cells = std::vector<std::vector<UICell>>(num_of_cells_to_display.x, std::vector<UICell>(num_of_cells_to_display.y, UICell(current_cell)));

  for (size_t x = 0; x < num_of_cells_to_display.x; x++) {
    for (size_t y = 0; y < num_of_cells_to_display.y; y++) {
      cells[x][y].set_position(sf::Vector2f(
            ui_grid_pos.x + x * distance_between_each_print, 
            ui_grid_pos.y + y * distance_between_each_print
            )
          );
    }
  }
}

UI::UI(const sf::Font &font, sf::Vector2u window_size) : cell_margin(15), window_size(window_size) {
  title = sf::Text("Game of Life", font, FONT_SIZE);
  title = center_sfml_text_bounds(title);
  title.setFillColor(OFF_WHITE_COLOR);

  cells = std::vector<std::vector<UICell>>(1, std::vector<UICell>(1, UICell(sf::Vector2f(0, 0), 50)));

  update_ui_positioning();
}

void UI::event_handler(sf::Event event) {
  switch(event.type) {
    case sf::Event::Resized:
      {
        window_size = sf::Vector2u(event.size.width, event.size.height);
        update_ui_positioning();
      }
      break;
    case sf::Event::MouseMoved:
      for (auto &vec_cells : cells) {
        for (auto &cell : vec_cells) {
          cell.mouse_handler(event.mouseButton);
        }
      }
      break;
    default:
      break;
  }
}

void UI::update() {
  for (auto &vec_cells : cells) {
    for (auto &cell : vec_cells) {
      cell.update();
    }
  }
}

void UI::render(sf::RenderWindow &rw) {
  rw.draw(title);

  for (const auto &vec_cells : cells) {
    for (const auto &cell : vec_cells) {
      cell.render(rw);
    }
  }
}
