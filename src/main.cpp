#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>

#include "ui.h"
#include "ui_constants.h"
#include "game_of_life.h"

template<typename T>
sf::Vector2<T> rect_size(sf::Rect<T> rect) {
  return sf::Vector2<T>(rect.width, rect.height);
}

std::optional<sf::Font> load_font() {
  sf::Font font;
  if (!font.loadFromFile("Manrope-VariableFont_wght.ttf")) {
    printf("Error loading font file Manrop-VariableFont_wght.ttf\nAborting");
    return {};
  }

  return font;
}

sf::Text center_text_origin(sf::Text text) {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width/2.0f,
      textRect.top  + textRect.height/2.0f);

  return text;
}

int main() {
  // Crashes if font fails to load
  const sf::Font font = load_font().value();
  sf::RenderWindow window(sf::VideoMode(800, 600), "Game Of Life");
  window.setVerticalSyncEnabled(false);
  
  UI ui(font, sf::Vector2u(800, 600));
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::Resized: 
          {
            const sf::Vector2u window_size(event.size.width, event.size.height);

            if (window_size.x < 200 || window_size.y < 200) continue;

            window.setView(sf::View(sf::FloatRect(0.f, 0.f, window_size.x, window_size.y)));

            ui.event_handler(event);
          }
          break;
        case sf::Event::MouseButtonPressed:
          {
          auto active_cells = ui.get_cell_locations();

          // TODO: Need to activate cells and pause if needed
          }
        case sf::Event::MouseMoved:
          ui.event_handler(event);
          break;
        default:
          break;
      }
    }
    ui.update();
    // TODO: set cell locations need to occur here in case of unpaused updates

    window.clear(OFF_BLACK_COLOR);
    ui.render(window);
    window.display();
  }

  return 0;
}
