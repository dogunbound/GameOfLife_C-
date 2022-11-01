#include "ui.h"
#include "ui_constants.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdio>
#include <limits.h>
#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <ctime>


void UI::update_ui_positioning() {
  set_title_position();
  set_coord_position();
  update_cell_positions();
  pause_play_button.reposition_to_screen_size(window_size);
}

void UI::set_coord_position() {
  sf::Vector2f pos = sf::Vector2f(
      15,
      25
      );
  coordinates.setPosition(pos);
}

void UI::set_coord_text() {
  coordinates.setString("(" + std::to_string(game_view_box.left) + ", " + std::to_string(game_view_box.top) + ")");
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

  const auto ui_grid_pos = sf::Vector2u(
      center_of_screen.x - corrected_grid_size.x / 2,
      center_of_screen.y - corrected_grid_size.y / 2
      );

  cells = std::vector<std::vector<UICell>>(num_of_cells_to_display.x, std::vector<UICell>(num_of_cells_to_display.y, UICell(current_cell)));
  cells_to_render = std::list<UICell*>();

  for (size_t x = 0; x < num_of_cells_to_display.x; x++) {
    for (size_t y = 0; y < num_of_cells_to_display.y; y++) {
      cells[x][y].set_position(sf::Vector2f(
            ui_grid_pos.x + x * distance_between_each_print, 
            ui_grid_pos.y + y * distance_between_each_print
            )
          );
      if (cells[x][y].is_border_displayed()) cells_to_render.push_back(&cells[x][y]);
    }
  }

  game_view_box.width = num_of_cells_to_display.x;
  game_view_box.height = num_of_cells_to_display.y;

  hovered_cells = std::list<UICell*>();
}

UI::UI(const sf::Font &font, sf::Vector2u window_size) : 
  mouse_is_dragging(false),
  cell_margin(15),
  window_size(window_size),
  last_update(clock()),
  pause_play_button(PausePlayButton(window_size)),
  game_view_box(sf::Rect<unsigned int>(UINT_MAX / 2, UINT_MAX / 2, 1, 1)) {

    title = sf::Text("Game of Life", font, FONT_SIZE);
    title = center_sfml_text_bounds(title);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(OFF_WHITE_COLOR);
    
    coordinates = sf::Text("N/A", font, FONT_SIZE / 2);
    set_coord_text();

    cells = std::vector<std::vector<UICell>>(1, std::vector<UICell>(1, UICell(sf::Vector2f(0, 0), 50)));
    cell_margin = 50 * 0.3f;

    update_ui_positioning();
  }


std::list<UICell*> UI::cells_near_mouse(const sf::Vector2i &mouse_pos) {
  const int distance_between_each_print = cell_margin + cells[0][0].get_cell_border_size().x;
  const sf::Vector2i top_left_cell_pos(cells[0][0].global_bounds().left, cells[0][0].global_bounds().top);

  const unsigned int min_x = std::max(((mouse_pos.x - top_left_cell_pos.x) / distance_between_each_print) - 1, 0);
  const unsigned int max_x = std::min((unsigned int) ((mouse_pos.x - top_left_cell_pos.x) / distance_between_each_print) + 1, (unsigned int) cells.size());
  const unsigned int min_y = std::max(((mouse_pos.y - top_left_cell_pos.y) / distance_between_each_print) - 1, 0);
  const unsigned int max_y = std::min((unsigned int) ((mouse_pos.y - top_left_cell_pos.y) / distance_between_each_print) + 1, (unsigned int) cells[0].size());
  std::list<UICell*> cells_near_mouse;
  for (unsigned int x = min_x; x < max_x; x++) {
    for (unsigned int y = min_y; y < max_y; y++) {
      cells_near_mouse.push_back(&cells[x][y]);
    }
  }

  return cells_near_mouse;
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
      for (auto iter = hovered_cells.begin(); iter != hovered_cells.end(); iter++) {
        (*iter)->mouse_handler(event.mouseMove);
        if (!(*iter)->get_is_hover()) {
          iter = hovered_cells.erase(iter);
        }
      }

      for (auto &cell : cells_near_mouse(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) {
        if (cell != nullptr) { 
          cell->mouse_handler(event.mouseMove);
          if (mouse_is_dragging && cell->get_is_hover() && hovered_cells.back() != cell) cell->set_active(!cell->get_is_active());
          if (cell->get_is_hover() && hovered_cells.back() != cell) hovered_cells.push_back(cell);
        }
      }

      pause_play_button.mouse_handler(event.mouseMove);
      break;
    case sf::Event::MouseButtonReleased:
      mouse_is_dragging = false;
      break;
    case sf::Event::MouseButtonPressed:
      mouse_is_dragging = true;
      for (auto &cell : cells_near_mouse(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
        if (cell != nullptr) {
          cell->mouse_handler(event.mouseButton);
          if (cell->get_is_hover() && hovered_cells.back() != cell) hovered_cells.push_back(cell);
        }
      }

      pause_play_button.mouse_handler(event.mouseButton);
      break;
    case sf::Event::MouseWheelScrolled:
      if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && 
          cells[0][0].get_regular_cell_size() + 0.1 * cells[0][0].get_regular_cell_size() * event.mouseWheelScroll.delta < 100
         ) {
        if (cells[0][0].get_regular_cell_size() <= 2 && event.mouseWheelScroll.delta <= 0) break;
        cells = std::vector<std::vector<UICell>>(1, 
            std::vector<UICell>(1, 
              UICell(sf::Vector2f(0, 0), cells[0][0].get_regular_cell_size() + 0.1 * cells[0][0].get_regular_cell_size() * event.mouseWheelScroll.delta)
              )
            );
        cell_margin = cells[0][0].get_regular_cell_size() * 0.3f;

        const sf::Vector2u game_view_box_center(game_view_box.width / 2 + game_view_box.left, game_view_box.height / 2 + game_view_box.top);
        const sf::Vector2u grid_size(window_size.x - 25, window_size.y - 100);
        const unsigned int distance_between_each_print = cells[0][0].get_regular_cell_size() + cell_margin;
        const sf::Vector2u num_of_cells_to_display(
            floor((float) grid_size.x / (float) distance_between_each_print),
            floor((float) grid_size.y / (float) distance_between_each_print)
            );

        game_view_box.left = game_view_box_center.x - num_of_cells_to_display.x / 2;
        game_view_box.top = game_view_box_center.y - num_of_cells_to_display.y / 2;
      } else if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        const int scroll_increment_amount = (50 / cells[0][0].get_regular_cell_size()) < 1 ? 1 : (50 / cells[0][0].get_regular_cell_size());
        game_view_box.top += scroll_increment_amount * -1 * (int) event.mouseWheelScroll.delta;
      } else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
        const int scroll_increment_amount = (50 / cells[0][0].get_regular_cell_size()) < 1 ? 1 : (50 / cells[0][0].get_regular_cell_size());
        game_view_box.left += scroll_increment_amount * -1 * (int) event.mouseWheelScroll.delta;
      }

      update_ui_positioning();
      break;
    default:
      break;
  }

  set_coord_text();
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

  cells_to_render = std::list<UICell*>();
  for (auto &vec_cells : cells) {
    for (auto &cell : vec_cells) {
      cell.update();
      if (cell.get_is_active() || cell.is_border_displayed()) cells_to_render.push_back(&cell);
    }
  }
  pause_play_button.update();

  last_update = clock();
}

void UI::render(sf::RenderWindow &rw) {
  for (const auto &cell : cells_to_render) cell->render(rw);
  rw.draw(coordinates);
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
