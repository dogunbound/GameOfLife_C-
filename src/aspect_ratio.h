#include <SFML/System/Vector2.h>
#include <optional>
#include <cmath>
#include <list>

class AspectRatio {
  private:
    sf::Vector2f aspect_ratio;
    sf::Vector2f base_resolution;
    sf::Vector2f computed_resolution;
    sf::Vector2f current_resolution;

  private:
    AspectRatio(sf::Vector2f aspect_ratio, sf::Vector2f base_resolution) : aspect_ratio(aspect_ratio), base_resolution(base_resolution) {
      computed_resolution = sf::Vector2f(0.0, 0.0);
      current_resolution = sf::Vector2f(0.0, 0.0);
    }

  public:
    static std::optional<AspectRatio> create(sf::Vector2f aspect_ratio, sf::Vector2f base_resolution) {
      if (base_resolution.y * aspect_ratio.x / aspect_ratio.y != base_resolution.x) {
        printf("Base_resolution's aspect_ratio does not match given aspect_ratio");
        return {};
      }

      return AspectRatio(aspect_ratio, base_resolution);
    }

    sfVector2i relative_mouse_coords(sfVector2i mouse_pos) {
      return sfVector2i(
          computed_resolution.x / current_resolution.x * mouse_pos.x,
          computed_resolution.y / current_resolution.y * mouse_pos.y
          );
    }

    void compute_resolution() {
      const auto compute_radian_from_vec = [](sf::Vector2f vec) { return atan (vec.x / vec.y); };
      const auto compute_area = [](sf::Vector2f vec) { return vec.x * vec.y; };

      const float target_radian = compute_radian_from_vec(current_resolution);
      const float target_area = compute_area(base_resolution);
      sf::Vector2f resolution = base_resolution;
      float current_radian = compute_radian_from_vec(resolution);

      std::list<sf::Vector2f> prev_resolutions;
      while (true) {
        if (target_radian < current_radian) {
          if (target_area > compute_area(resolution)) {
            resolution.y += 1.0;
          } else {
            resolution.x -= 1.0;
          }
        } else {
          if (target_area > compute_area(resolution)) {
            resolution.x += 1.0;
          } else {
            resolution.y -= 1.0;
          }
        }

        current_radian = compute_radian_from_vec(resolution);

        if (prev_resolutions.size() > 4) {
          prev_resolutions.pop_back();
        }

        for (auto prev_resolution : prev_resolutions) {
          if (prev_resolution == resolution) {
            break;
          }
        }

        prev_resolutions.push_front(resolution);
      }

      computed_resolution = resolution;
    }

    // Getters
    sf::Vector2f get_aspect_ratio() { return aspect_ratio; }
    sf::Vector2f get_base_resolution() { return base_resolution; }
    sf::Vector2f get_computed_resolution() { return computed_resolution; }
    sf::Vector2f get_current_resolution() { return current_resolution; }
};
