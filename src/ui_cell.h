#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "ui_constants.h"

class UICell {
  private:
    bool is_active;
    bool is_hover;
    sf::RectangleShape cell_border;
    const float cell_size;

    void mouse_handler(sf::Vector2f mouse_pos, bool left_button_pressed) {
      std::cout << cell_border.getPosition().x << " " << cell_border.getPosition().y << std::endl;
      is_hover = cell_border.getGlobalBounds().contains(mouse_pos);
      is_active = left_button_pressed && is_hover ? !is_active : is_active;
    }
  public:
    void set_position(const sf::Vector2f &pos) { cell_border.setPosition(pos); }
    const sf::Vector2f get_cell_border_size() const { return cell_border.getSize(); }
    const sf::FloatRect global_bounds() const { return cell_border.getGlobalBounds(); }
    const float get_regular_cell_size() const { return cell_size; }

    UICell() : is_hover(false), is_active(false), cell_size(0) {}
    UICell(const sf::Vector2f &coord, const float &cell_size) : is_hover(false), is_active(false), cell_size(cell_size) {
      cell_border = sf::RectangleShape(sf::Vector2f(cell_size, cell_size));
      cell_border.setPosition(coord);
      cell_border.setOutlineThickness(1);
      cell_border.setOutlineColor(OFF_WHITE_COLOR);
      cell_border.setFillColor(OFF_BLACK_COLOR);
      
      std::cout << cell_border.getPosition().x << " " << cell_border.getPosition().y << std::endl;
      printf("%d %d\n", cell_border.getPosition().x, cell_border.getPosition().y);
    }
    UICell(const UICell &ui_cell) : is_active(ui_cell.is_active), is_hover(ui_cell.is_hover), cell_border(ui_cell.cell_border), cell_size(ui_cell.cell_size) {}

    void mouse_handler(sf::Event::MouseButtonEvent mouse) {
      mouse_handler(sf::Vector2f(mouse.x, mouse.y), false);
    }

    void update() {
      if (is_hover) {
        const float hover_cell_size = cell_size * 0.5;

        if (cell_border.getSize().x < hover_cell_size) {
          cell_border.setSize(sf::Vector2f(
                cell_border.getSize().x + 2,
                cell_border.getSize().y + 2
                )
              );

          cell_border.setPosition(sf::Vector2f(
                cell_border.getPosition().x - 1,
                cell_border.getPosition().y - 1
                )
              );
        } else if (cell_border.getSize().x > cell_size) {
          cell_border.setSize(sf::Vector2f(
                cell_border.getSize().x - 2,
                cell_border.getSize().y - 2
                )
              );

          cell_border.setPosition(sf::Vector2f(
                cell_border.getPosition().x + 1,
                cell_border.getPosition().y + 1
                )
              );
        }
      }
    }

    void render(sf::RenderWindow &rw) const {
      rw.draw(cell_border);
    }
};