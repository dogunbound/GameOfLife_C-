#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <time.h>

#include "ui_constants.h"
#include "ui_cell.h"
#include "pause_play.h"
#include "cell.h"
#include "helper.h"

class UI {
  private:
    static constexpr unsigned int DEFAULT_CELL_SIZE = 50;
    static constexpr char* TITLE_TEXT = "Game Of Life";
    static constexpr unsigned int FONT_SIZE = 32;
    static constexpr float MILLIS_BETWEEN_UPDATES = 10.f;
    sf::Vector2u window_size;
    sf::Text title;
    sf::Rect<unsigned int> game_view_box;
    std::vector<std::vector<UICell>> cells;
    std::list<UICell*> last_cells_to_hover;
    UICell *last_cell_to_change_activity_status;
    unsigned int cell_margin;
    std::unordered_set<Cell> active_cells;
    PausePlayButton pause_play_button;
    bool is_mouse_dragging;
    clock_t time_of_last_update;
  public:
  private:
    bool is_cell_in_valid_state_to_change(const UICell &);
    sf::Vector2u game_coord_to_ui_coord(const Cell);
    std::list<std::tuple<sf::Vector2u, UICell*>> cells_near_mouse(const sf::Event::MouseMoveEvent);
    void activate_cell(UICell &, const sf::Vector2u);
    void deactivate_cell(UICell &, const sf::Vector2u);
    void readjust_all_ui_cell_positions();
    void readjust_all_ui_positions();
    void readjust_title_position();
    void readjust_play_button_position();
  public:
    UI(const sf::Font &font, sf::Vector2u window_size);
    const sf::Rect<unsigned int> get_game_view_box() const { return game_view_box; }

    std::list<std::tuple<AddOrRemoveCell, Cell>> event_handler(const sf::Event &);
    void set_cell_locations_in_game_view_box(const std::list<std::tuple<AddOrRemoveCell, Cell>> &changed_cells);
    const bool is_paused() const { return pause_play_button.get_is_paused(); }

    void update();
    void render(sf::RenderWindow &rw);
};
