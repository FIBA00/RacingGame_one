#ifndef HUD_H
#define HUD_H

// import types
#include "types.h"

void hud_init(GameState &game);
void hud_destroy(GameState &game);
void hud_draw_stats(const Rect &car);
void hud_draw_minimap(const Rect &car, const Obstacle obstacles[], int obs_count);

#endif