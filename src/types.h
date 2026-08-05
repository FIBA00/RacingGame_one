#ifndef TYPES_H
#define TYPES_H

#include <cute.h>
using namespace Cute;

struct Rect
{
    float x_pos, y_pos;
    float width, height;
    CF_Color color;
    float thicknes;
    float heading;
    float speed;
    float vel_x, vel_y;
    int gear;           // 1 - 5, starts in 1
    float rpm;          // 0 - 8000
    float engine_power; // varies by gear
};

struct Obstacle
{
    float x_pos, y_pos;
    float radius;
    CF_Color color;
};

struct GameState
{
    Rect car;
    Obstacle obstacles[5];
    const char *font_name; // String identifier not a struct
    bool fonts_loaded;
};

enum GamePhase
{
    PHASE_LOADING,
    PHASE_PLAYING,
};

#endif