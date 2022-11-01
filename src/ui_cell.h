#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "ui_constants.h"

class UICell {
  private:
    const static int CELL_SIZE_TOO_SMALL = 15;
    bool is_active;
    bool is_hover;
    sf::RectangleShape cell_border;
    const float cell_size;

    void mouse_handler(sf::Vector2f mouse_pos, bool left_button_pressed) {
      is_hover = cell_border.getGlobalBounds().contains(mouse_pos);
      is_active = left_button_pressed && is_hover ? !is_active : is_active;
    }

  public:
    const bool is_border_displayed() { return CELL_SIZE_TOO_SMALL < get_regular_cell_size(); }
    const bool get_is_active() const { return is_active; }
    const bool get_is_hover() const { return is_hover; }
    void set_active(bool is_active) { this->is_active = is_active; }
    void set_position(const sf::Vector2f &pos) { cell_border.setPosition(pos); }
    const sf::Vector2f get_cell_border_size() const { return cell_border.getSize(); }
    const sf::FloatRect global_bounds() const { return cell_border.getGlobalBounds(); }
    const float get_regular_cell_size() const { return cell_size; }

    UICell() : is_hover(false), is_active(false), cell_size(0) {}
    UICell(const sf::Vector2f &coord, const float &cell_size) : is_hover(false), is_active(false), cell_size(cell_size) {
      cell_border = sf::RectangleShape(sf::Vector2f(cell_size, cell_size));
      cell_border.setPosition(coord);
      cell_border.setOutlineThickness(cell_size > CELL_SIZE_TOO_SMALL ? 1 : 0);
      cell_border.setOutlineColor(OFF_WHITE_COLOR);
      cell_border.setFillColor(OFF_BLACK_COLOR);
    }

    UICell(const UICell &ui_cell) : is_active(ui_cell.is_active), is_hover(ui_cell.is_hover), cell_border(ui_cell.cell_border), cell_size(ui_cell.cell_size) {}

    void mouse_handler(sf::Event::MouseMoveEvent mouse) {
      mouse_handler(sf::Vector2f(mouse.x, mouse.y), false);
    }

    void mouse_handler(sf::Event::MouseButtonEvent mouse) {
      mouse_handler(sf::Vector2f(mouse.x, mouse.y), mouse.button == sf::Mouse::Left);
    }

    void update() {
      const float diff_change = cell_size * .01;
      if (is_hover) {
        const float hover_cell_size = cell_size * 1.1;

        if (cell_border.getSize().x < hover_cell_size) {
          cell_border.setSize(sf::Vector2f(
                cell_border.getSize().x + diff_change * 2,
                cell_border.getSize().y + diff_change * 2
                )
              );

          cell_border.setPosition(sf::Vector2f(
                cell_border.getPosition().x - diff_change,
                cell_border.getPosition().y - diff_change
                )
              );
        }       
      } else if (cell_border.getSize().x > cell_size) {
        cell_border.setSize(sf::Vector2f(
              cell_border.getSize().x - diff_change * 2,
              cell_border.getSize().y - diff_change * 2
              )
            );

        cell_border.setPosition(sf::Vector2f(
              cell_border.getPosition().x + diff_change,
              cell_border.getPosition().y + diff_change
              )
            ); 
      }
      cell_border.setFillColor(is_active ? OFF_WHITE_COLOR : OFF_BLACK_COLOR);
    }

    void render(sf::RenderWindow &rw) const {
      rw.draw(cell_border);
    }
};
