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
#include <tuple>

sf::Text &center_sfml_text_bounds(sf::Text &text) {
  sf::FloatRect textRect = text.getLocalBounds();

  text.setOrigin(
      textRect.left + textRect.width/2.0f,
      textRect.top + textRect.height/2.0f
      );

  return text;
}

// PRIVATE FUNCTIONS ***************************************************
//
void UI::readjust_all_ui_positions() {
  readjust_all_ui_cell_positions();
  readjust_title_position();
  readjust_play_button_position();
}

void UI::readjust_play_button_position() {
  pause_play_button.reposition_to_screen_size(window_size);
}

void UI::readjust_title_position() {
  const float amt_push_downwards = title.getLocalBounds().height / 2 + 10;
  sf::Vector2f pos = sf::Vector2f(
      window_size.x / 2.f,
      amt_push_downwards
      );
  title.setPosition(pos);
}

void UI::readjust_all_ui_cell_positions() {
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

  last_cell_to_change_activity_status = nullptr;
  last_cells_to_hover = std::list<UICell*>();
}

sf::Vector2u UI::game_coord_to_ui_coord(const Cell cell) {
  return sf::Vector2u(cell.x - game_view_box.left, cell.y - game_view_box.top);
}

std::list<std::tuple<sf::Vector2u, UICell*>> UI::cells_near_mouse(const sf::Event::MouseMoveEvent mouse_pos) {
  const int distance_between_each_print = cell_margin + cells[0][0].get_cell_border_size().x;
  const sf::Vector2i top_left_cell_pos(cells[0][0].global_bounds().left, cells[0][0].global_bounds().top);

  const unsigned int min_x = std::max(((mouse_pos.x - top_left_cell_pos.x) / distance_between_each_print) - 1, 0);
  const unsigned int max_x = std::min((unsigned int) ((mouse_pos.x - top_left_cell_pos.x) / distance_between_each_print) + 1, (unsigned int) cells.size());
  const unsigned int min_y = std::max(((mouse_pos.y - top_left_cell_pos.y) / distance_between_each_print) - 1, 0);
  const unsigned int max_y = std::min((unsigned int) ((mouse_pos.y - top_left_cell_pos.y) / distance_between_each_print) + 1, (unsigned int) cells[0].size());
  std::list<std::tuple<sf::Vector2u, UICell*>> cells_near_mouse;
  for (unsigned int x = min_x; x < max_x; x++) {
    for (unsigned int y = min_y; y < max_y; y++) {
      cells_near_mouse.push_back(std::make_tuple(sf::Vector2u(x, y), &cells[x][y]));
    }
  }

  return cells_near_mouse;
}

void UI::activate_cell(UICell &cell, const sf::Vector2u ui_grid_pos) {
  cell.set_active(true);
  active_cells.insert(Cell(ui_grid_pos.x + game_view_box.left, ui_grid_pos.y + game_view_box.top));

  last_cell_to_change_activity_status = &cell;
}

void UI::deactivate_cell(UICell &cell, const sf::Vector2u ui_grid_pos) {
  cell.set_active(false);
  active_cells.erase(Cell(ui_grid_pos.x + game_view_box.left, ui_grid_pos.y + game_view_box.top));

  last_cell_to_change_activity_status = &cell;
}

bool UI::is_cell_in_valid_state_to_change(const UICell &cell) {
  return &cell != last_cell_to_change_activity_status;
}

// PUBLIC FUNCTIONS ***********************************
//
std::list<std::tuple<AddOrRemoveCell, Cell>> UI::event_handler(const sf::Event &event) {
  std::list<std::tuple<AddOrRemoveCell, Cell>> add_or_removed_cells;
  switch(event.type) {
    case sf::Event::Resized:
      window_size = sf::Vector2u(event.size.width, event.size.height);
      readjust_all_ui_positions();
      break;
    case sf::Event::MouseMoved:
      for (auto iter = last_cells_to_hover.begin(); iter != last_cells_to_hover.end(); iter++) {
        (*iter)->mouse_handler(event.mouseMove);
        if (!(*iter)->get_is_hover()) {
          iter = last_cells_to_hover.erase(iter);
        }
      }
      for (auto &tuple : cells_near_mouse(event.mouseMove)) {
        auto [ui_grid_pos, cell] = tuple;
        cell->mouse_handler(event.mouseMove);
        if (is_mouse_dragging && cell->get_is_hover() && is_cell_in_valid_state_to_change(*cell)) {
          if (cell->get_is_active()) {
            deactivate_cell(*cell, ui_grid_pos);
            add_or_removed_cells.push_back(std::make_tuple(AddOrRemoveCell::Remove, Cell(ui_grid_pos)));
          } else {
            activate_cell(*cell, ui_grid_pos);
            add_or_removed_cells.push_back(std::make_tuple(AddOrRemoveCell::Add, Cell(ui_grid_pos)));
          }
        }

        if (cell->get_is_hover()) {
          last_cells_to_hover.push_back(cell);
        }
      }

      pause_play_button.mouse_handler(event.mouseMove);
      break;
    case sf::Event::MouseButtonReleased:
      is_mouse_dragging = false;
      break;
    case sf::Event::MouseButtonPressed:
      is_mouse_dragging = true;
      for (auto &tuple : cells_near_mouse(event.mouseMove)) {
        auto [ui_grid_pos, cell] = tuple;
        cell->mouse_handler(event.mouseButton);
        if (is_mouse_dragging && cell->get_is_hover() && is_cell_in_valid_state_to_change(*cell)) {
          if (cell->get_is_active()) {
            deactivate_cell(*cell, ui_grid_pos);
            add_or_removed_cells.push_back(std::make_tuple(AddOrRemoveCell::Remove, Cell(ui_grid_pos)));
          } else {
            activate_cell(*cell, ui_grid_pos);
            add_or_removed_cells.push_back(std::make_tuple(AddOrRemoveCell::Add, Cell(ui_grid_pos)));
          }
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

      readjust_all_ui_cell_positions();
      break;
    default:
      break;
  }

  return add_or_removed_cells;
}

void UI::set_cell_locations_in_game_view_box(const std::list<std::tuple<AddOrRemoveCell, Cell>> &changed_cells) {
  for (const auto &tuple : changed_cells) { 
    const auto[add_or_remove, cell] = tuple;
    const sf::Vector2u ui_coord = game_coord_to_ui_coord(cell);

    if (add_or_remove == AddOrRemoveCell::Add) activate_cell(cells[ui_coord.x][ui_coord.y], ui_coord);
    else deactivate_cell(cells[ui_coord.x][ui_coord.y], ui_coord);
  }
}

void UI::render(sf::RenderWindow &rw) {
  if (cells[0][0].is_border_displayed()) {
    for (auto &vec_cells : cells) {
      for (auto &cell : vec_cells) {
        cell.render(rw);
      }
    }
  } else {
    for (const auto &active_cell : active_cells) {
      const auto ui_coord = game_coord_to_ui_coord(active_cell);
      cells[ui_coord.x][ui_coord.y].render(rw);
    }
  }

  pause_play_button.render(rw);
  rw.draw(title);
}

void UI::update() {
  if (((float) clock() - (float) time_of_last_update) / (float) CLOCKS_PER_SEC * 1000.0f < MILLIS_BETWEEN_UPDATES)
    return;

  if (cells[0][0].is_border_displayed()) {
    for (auto &vec_cells : cells) {
      for (auto &cell : vec_cells) {
        cell.update();
      }
    }
  }
  pause_play_button.update();

  time_of_last_update = clock();
}

UI::UI(const sf::Font &font, sf::Vector2u window_size) : pause_play_button(PausePlayButton(window_size)) {
  is_mouse_dragging = false;
  this->window_size = window_size;
  last_cells_to_hover = std::list<UICell*>();

  title = sf::Text(TITLE_TEXT, font, FONT_SIZE);
  title = center_sfml_text_bounds(title);
  title.setStyle(sf::Text::Bold);
  title.setFillColor(OFF_WHITE_COLOR);

  cells = std::vector<std::vector<UICell>>(1, std::vector<UICell>(1, UICell(sf::Vector2f(0, 0), DEFAULT_CELL_SIZE)));
  cell_margin = DEFAULT_CELL_SIZE * 0.3f;

  time_of_last_update = clock();

  readjust_all_ui_cell_positions();
}
