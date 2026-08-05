#include "loading.h"
#include "config.h"
#include <cstdio>

void loading_init(LoadingState &state, float duration)
{
    state.elapsed = 0.0f;
    state.duration = duration;
    state.done = false;
};

void loading_update(LoadingState &state, float delta_time)
{
    state.elapsed += delta_time;
    if (state.elapsed >= state.duration)
    {
        state.done = true;
    }
};

void loading_draw(const LoadingState &state)
{
    // Dark semi-transparent overlay (full screen)
    draw_push_color(CF_Color{0.0f, 0.0f, 0.0f, 0.9f});
    draw_quad_fill(make_aabb(V2(0, 0), WIN_WIDTH, WIN_HEIGHT), 0);
    draw_pop_color();

    // center box (loading window)
    float box_w = 400.0f;
    float box_h = 200.0f;
    float box_x = (WIN_WIDTH - box_w) / 2.0f;
    float box_y = (WIN_HEIGHT - box_h) / 2.0f;

    // Loading box background
    draw_push_color(CF_Color{0.2f, 0.2f, 0.2f, 0.95f});
    draw_quad_fill(make_aabb(V2(box_x, box_y), box_w, box_h), 0);
    draw_pop_color();

    // Loading box border
    draw_push_color(color_white());
    draw_box(V2(box_x + box_w / 2, box_y + box_h / 2), box_w, box_h, 3.0f);
    draw_pop_color();

    // Title text
    char title[128];
    snprintf(title, sizeof(title), "Racing Game One");
    draw_push_color(color_cyan());
    cf_push_font("main_font");
    cf_push_font_size(32);
    cf_draw_text(title, V2(box_x + 50, box_y + 40), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();

    // Progress bar background
    float bar_w = 300.0f;
    float bar_h = 20.0f;
    float bar_x = box_x + (box_w - bar_w) / 2.0f;
    float bar_y = box_y + 120.0f;

    draw_push_color(CF_Color{0.3f, 0.3f, 0.3f, 1.0f});
    draw_quad_fill(make_aabb(V2(bar_x, bar_y), bar_w, bar_h), 0);
    draw_pop_color();

    // Progress bar fill (based on elapsed time)
    float progress = state.elapsed / state.duration;
    if (progress > 1.0f)
    {
        progress = 1.0f;
    };
    float fill_w = bar_w * progress;

    draw_push_color(color_green());
    draw_quad_fill(make_aabb(V2(bar_x, bar_y), fill_w, bar_h), 0);
    draw_pop_color();

    // Progress Text
    char progress_text[128];
    snprintf(progress_text, sizeof(progress_text), "Loading... %.0f%%", progress * 100.0f);
    draw_push_color(color_white());
    cf_push_font("main_font");
    cf_push_font_size(16);
    cf_draw_text(progress_text, V2(bar_x, bar_y + 30), -1);
    cf_pop_font_size();
    cf_pop_font();
    draw_pop_color();
};
