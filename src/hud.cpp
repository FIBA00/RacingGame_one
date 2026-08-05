#include "hud.h"
#include "config.h"
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

bool file_exists(const char *path)
{

    return fs::exists(path);
};

void hud_init(GameState &game)
{
    game.fonts_loaded = false;
    game.font_name = nullptr;

    // check if file exists BEFORE trying to load it
    if (!file_exists("assets/font.ttf"))
    {
        printf("Warning: Could not load font from assets/font.ttf\n");
        printf("Continuing without font rendering.\n");
        return;
    }

    // Register the default font with CF, Path is relative to executable
    // if font file doesnt exist , CF_Ressult will be an error - check is_error().
    CF_Result result = cf_make_font("assets/font.ttf", "main_font");

    if(is_error(result)){
        printf("Warning: Could not load font from assets/font.ttf\n");
        printf("Error: %s\n", result.details);
        game.fonts_loaded = false;
        game.font_name = nullptr;

        // fall back to whatever default font CF has - dont crash
    }
    else {
        game.fonts_loaded = true;
        game.font_name = "main_font";
    }
};

void hud_destroy(GameState &game) {
    if (game.fonts_loaded && game.font_name)
    {
        cf_destroy_font(game.font_name);
    }
};

void hud_draw_stats(const GameState &game)
{
    if (!game.fonts_loaded || !game.font_name)
        return;

    const Rect &car = game.car;
    float speed_mag = sqrtf(car.vel_x * car.vel_x + car.vel_y * car.vel_y);
    char buffer[256];

    // Gear
    snprintf(buffer, sizeof(buffer), "Gear: %d", car.gear);
    draw_push_color(color_white());
    cf_push_font(game.font_name);
    cf_push_font_size(22);
    cf_draw_text(buffer, V2(50, 50), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();

    // Speed
    snprintf(buffer, sizeof(buffer), "Speed: %.0f px/s", speed_mag);
    draw_push_color(color_cyan());
    cf_push_font(game.font_name);
    cf_push_font_size(22);
    cf_draw_text(buffer, V2(50, 100), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();

    // RPM
    snprintf(buffer, sizeof(buffer), "RPM: %.0f", car.rpm);
    draw_push_color(color_yellow());
    cf_push_font(game.font_name);
    cf_push_font_size(22);
    cf_draw_text(buffer, V2(50, 150), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();

    // Position
    snprintf(buffer, sizeof(buffer), "Pos: (%.0f, %.0f)", car.x_pos, car.y_pos);
    draw_push_color(color_magenta());
    cf_push_font(game.font_name);
    cf_push_font_size(18);
    cf_draw_text(buffer, V2(50, 200), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();
};

void hud_draw_minimap(const Rect &car, const Obstacle obstacles[], int obs_count){
    // minimap is 200x150 pixels , top-right corner with padding
    float mm_x = WIN_WIDTH - 220;
    float mm_y = 20;
    float mm_w = 200;
    float mm_h = 150;

    // draw minimap background (semi-transparent dark)
    draw_push_color(CF_Color{0, 0, 0, 0.7f});
    draw_quad_fill(make_aabb(V2(mm_x, mm_y), mm_w, mm_h), 0);
    draw_pop_color();

    // draw minimap border
    draw_push_color(color_white());
    draw_box(V2(mm_x + mm_w / 2, mm_y + mm_h / 2), mm_w, mm_h, 2.0f);
    draw_pop_color();

    // scale factor : world to minimap pixels
    float sx = mm_w / WORLD_WIDTH;
    float sy = mm_h / WORLD_HEIGHT;

    // center of minimap in screen space
    float cx = mm_x + mm_w / 2;
    float cy = mm_y + mm_h / 2;

    // Draw obstacles (small circles)
    for (int i = 0; i < obs_count; ++i)
    {
        float ox = cx + obstacles[i].x_pos * sx;
        float oy = cy + obstacles[i].y_pos * sy;

        draw_push_color(obstacles[i].color);
        draw_circle_fill(make_circle(V2(ox, oy), 3));
        draw_pop_color();
    }

    // draw car (small triangle pointing heading direction)'
    float cx_car = cx + car.x_pos * sx;
    float cy_car = cy + car.y_pos * sy;

    draw_push_color(color_red());
    draw_circle_fill(make_circle(V2(cx_car, cy_car), 5));
    draw_pop_color();

};
