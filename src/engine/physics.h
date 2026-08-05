#ifndef PHYSICS_H
#define PHYSICS_H
#include "types.h"

void physics_update_car(Rect &car, float delta_time);
void physics_apply_speed_cap(Rect &car, float max_speed);
void physics_resolve_circle_collision(Rect &car, const Obstacle &obs);
void physics_resolve_track_collision(Rect &car);

#endif