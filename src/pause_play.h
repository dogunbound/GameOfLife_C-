#pragma once

#include <SFML/System.h>
#include <SFML/Graphics.h>

#include "ui_constants.h"

class PausePlayButton {
  private:
    sf::CircleShape play_icon;
    sf::RectangleShape pause_icon_bar1;
    sf::RectangleShape pause_icon_bar2;
    sf::RectangleShape cell_outline;

    bool is_paused;
    bool is_hover;

    void mouse_handler(const sf::Vector2f &mouse_pos, bool left_button_pressed) {
      is_hover = cell_outline.getGlobalBounds().contains(mouse_pos);
      is_paused = is_hover && left_button_pressed ? !is_paused : is_paused;
    }

  public:
    const bool get_is_paused() const { return is_paused; }

    void mouse_handler(sf::Event::MouseMoveEvent mouse) {
      mouse_handler(sf::Vector2f(mouse.x, mouse.y), false);
    }

    void mouse_handler(sf::Event::MouseButtonEvent mouse) {
      mouse_handler(sf::Vector2f(mouse.x, mouse.y), mouse.button == sf::Mouse::Left);
    }

    void reposition_to_screen_size(const sf::Vector2u &window_size) {
      const sf::Vector2f correct_position(
          window_size.x / 2,
          window_size.y - cell_outline.getSize().y / 2 - 5
          );

      cell_outline.setPosition(correct_position);
      play_icon.setPosition(correct_position);

      pause_icon_bar1.setPosition(sf::Vector2f(
            correct_position.x - cell_outline.getSize().x / 6,
            correct_position.y
            )
          );
      pause_icon_bar2.setPosition(sf::Vector2f(
            correct_position.x + cell_outline.getSize().x / 6,
            correct_position.y
            )
          );
    }

    PausePlayButton(sf::Vector2u window_size) : is_paused(true), is_hover(false) {
      play_icon = sf::CircleShape(15, 3);
      play_icon.setRotation(90.f);
      play_icon.setFillColor(OFF_WHITE_COLOR);
      play_icon.setOrigin(
          play_icon.getLocalBounds().left + play_icon.getLocalBounds().width/2.0f,
          play_icon.getLocalBounds().top + play_icon.getLocalBounds().height/2.0f
          );

      const sf::RectangleShape pause_icon_bar(sf::Vector2f(4, 30));
      pause_icon_bar1 = sf::RectangleShape(pause_icon_bar);
      pause_icon_bar1.setFillColor(OFF_WHITE_COLOR);
      pause_icon_bar1.setOrigin(
          pause_icon_bar1.getLocalBounds().left + pause_icon_bar1.getLocalBounds().width/2.0f,
          pause_icon_bar1.getLocalBounds().top + pause_icon_bar1.getLocalBounds().height/2.0f
          );

      pause_icon_bar2 = sf::RectangleShape(pause_icon_bar);
      pause_icon_bar2.setFillColor(OFF_WHITE_COLOR);
      pause_icon_bar2.setOrigin(
          pause_icon_bar2.getLocalBounds().left + pause_icon_bar2.getLocalBounds().width/2.0f,
          pause_icon_bar2.getLocalBounds().top + pause_icon_bar2.getLocalBounds().height/2.0f
          );

      cell_outline = sf::RectangleShape(sf::Vector2f(50, 50));
      cell_outline.setFillColor(OFF_BLACK_COLOR);
      cell_outline.setOutlineThickness(1);
      cell_outline.setOutlineColor(OFF_WHITE_COLOR);
      cell_outline.setOrigin(
          cell_outline.getLocalBounds().left + cell_outline.getLocalBounds().width/2.0f,
          cell_outline.getLocalBounds().top + cell_outline.getLocalBounds().height/2.0f
          );

      reposition_to_screen_size(window_size);
    }

    void update() {
      const sf::Color hover_color(45, 50, 50);
      const int increment = 5;
      if (is_hover && cell_outline.getFillColor() != hover_color) {
        if (cell_outline.getFillColor().r >= hover_color.r ||
            cell_outline.getFillColor().g >= hover_color.g ||
            cell_outline.getFillColor().b >= hover_color.b) {
          cell_outline.setFillColor(hover_color);
        } else {
          cell_outline.setFillColor(sf::Color(
                cell_outline.getFillColor().r + increment,
                cell_outline.getFillColor().g + increment,
                cell_outline.getFillColor().b + increment
                )
              );
        }
      } else if (cell_outline.getFillColor() != OFF_BLACK_COLOR) {
        if (cell_outline.getFillColor().r > hover_color.r ||
            cell_outline.getFillColor().g > hover_color.g ||
            cell_outline.getFillColor().b > hover_color.b) {
          cell_outline.setFillColor(OFF_BLACK_COLOR);
        } else {
          const int r = cell_outline.getFillColor().r - increment;
          const int g = cell_outline.getFillColor().g - increment;
          const int b = cell_outline.getFillColor().b - increment;
          cell_outline.setFillColor(sf::Color(
                r < 0 ? 0 : r,
                g < 0 ? 0 : g,
                b < 0 ? 0 : r
                )
              );
        }
      }
    }

    void render(sf::RenderWindow &rw) {
      rw.draw(cell_outline);
      if (!is_paused) {
        rw.draw(pause_icon_bar2);
        rw.draw(pause_icon_bar1);
      } else {
        rw.draw(play_icon);
      }
    }
};
