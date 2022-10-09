#include <SFML/System/Vector2.h>

class AspectRatio {
  private:
    sfVector2f aspect_ratio;
    sfVector2f base_resolution;
    sfVector2f computed_resolution;
    sfVector2f current_resolution;

  private:
    AspectRatio(sfVector2f aspect_ratio, sfVector2f base_resolution) : aspect_ratio(aspect_ratio), base_resolution(base_resolution) {
      computed_resolution = sfVector2f(0.0, 0.0);
    }
};
