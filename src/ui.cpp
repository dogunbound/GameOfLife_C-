#include "ui.h"
#include "ui_constants.h"

#include <cstdio>
#include <limits.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <ctime>


void UI::update_ui_positioning() {
  set_title_position();
  update_cell_positions();
  pause_play_button.reposition_to_screen_size(window_size);
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
  const sf::Vector2u grid_size(window_size.x - 25, window_size.y - 100);
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

  game_view_box.width = num_of_cells_to_display.x;
  game_view_box.height = num_of_cells_to_display.y;
}

UI::UI(const sf::Font &font, sf::Vector2u window_size) : 
  cell_margin(15),
  window_size(window_size),
  last_update(clock()),
  pause_play_button(PausePlayButton(window_size)),
  game_view_box(sf::Rect<unsigned int>(UINT_MAX / 2, UINT_MAX / 2, 1, 1)) {

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
          cell.mouse_handler(event.mouseMove);
        }
      }

      pause_play_button.mouse_handler(event.mouseMove);
      break;
    case sf::Event::MouseButtonPressed:
      for (auto &vec_cells : cells) {
        for (auto &cell : vec_cells) {
          cell.mouse_handler(event.mouseButton);
        }
      }

      pause_play_button.mouse_handler(event.mouseButton);
    default:
      break;
  }
}

const std::list<Cell> UI::get_cell_locations() const {
  std::list<Cell> cells;

  for (size_t r = 0; r < this->cells.size(); r++) {
    for (size_t c = 0; c < this->cells[r].size(); c++) { 
      if (this->cells[r][c].get_is_active()) {
        cells.push_back(Cell(game_view_box.left + r, game_view_box.top + c));
      }
    }
  }

  return cells;
}

void UI::update() {
  if (((float) clock() - (float) last_update) / (float) CLOCKS_PER_SEC * 1000.0f < MILLIS_BETWEEN_UPDATES)
    return;

  for (auto &vec_cells : cells) {
    for (auto &cell : vec_cells) {
      cell.update();
    }
  }
  pause_play_button.update();

  last_update = clock();
}

void UI::render(sf::RenderWindow &rw) {
  for (const auto &vec_cells : cells) {
    for (const auto &cell : vec_cells) {
      cell.render(rw);
    }
  }
  rw.draw(title);
  pause_play_button.render(rw);
}

void UI::set_cell_locations(const std::list<Cell> &cells) {
  for (auto &vec_cells : this->cells) {
    for (auto &cell : vec_cells) {
      cell.set_active(false);
    }
  }

  for (const auto &cell : cells) {
    const sf::Vector2u ui_grid_coord(cell.x - game_view_box.left, cell.y - game_view_box.top);

    if (ui_grid_coord.x < this->cells.size() && ui_grid_coord.y < this->cells[0].size()) {
      this->cells[ui_grid_coord.x][ui_grid_coord.y].set_active(true);
    }
  }
}
