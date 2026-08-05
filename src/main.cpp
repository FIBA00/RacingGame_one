#include <cute.h>
#include "config.h"
#include "types.h"
#include "physics.h"
#include "render.h"
#include "hud.h"

using namespace Cute;

int main(int argc, char *argv[])
{
    CF_Result result = make_app("Racing Game One", 0, 0, 0, WIN_WIDTH, WIN_HEIGHT, CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT, argv[0]);
    if (is_error(result))
    {
        printf("Error: %s\n", result.details);
        return -1;
    }
    GamePhase phase = PHASE_LOADING;
    LoadingState loading = {};
    loading_init(loading, 3.0f); // 3 second loading screen

    GameState game = {};
    game.car = {
        .x_pos = 0,
        .y_pos = 0,
        .heading = CF_PI / 2.0f,
        .vel_x = 0,
        .vel_y = 0,
        .gear = 1,
        .rpm = 500.0f,
    };
    game.obstacles[0] = {500, 400, 30, color_magenta()};
    game.obstacles[1] = {-600, 800, 40, color_yellow()};
    game.obstacles[2] = {1000, -500, 25, color_orange()};
    game.obstacles[3] = {-800, -1200, 35, color_purple()};
    game.obstacles[4] = {200, 1500, 20, color_blue()};

    hud_init(game);

    while (app_is_running())
    {
        app_update();

        // background (screen space)
        draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
        draw_quad_fill(make_aabb(V2(0, 0), WIN_WIDTH, WIN_HEIGHT), 0);
        draw_pop_color();

        // Hud (Screen Space)
        hud_draw_stats(game.car);
        hud_draw_minimap(game.car, game.obstacles, OBS_COUNT);
        app_draw_onto_screen();
    }
    destroy_app();
    return 0;
}
