#include <cute.h>
#include "config.h"
#include "types.h"
#include "physics.h"
#include "render.h"
#include "hud.h"
#include "loading.h"

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

        if (phase == PHASE_LOADING)
        {
            loading_update(loading, CF_DELTA_TIME);

            // background (screen space)
            draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
            draw_quad_fill(make_aabb(V2(0, 0), WIN_WIDTH, WIN_HEIGHT), 0);
            draw_pop_color();

            // loading screen overlay
            loading_draw(loading);
            if (loading.done)
            {
                phase = PHASE_PLAYING;
            }
        }
        else if (phase == PHASE_PLAYING)
        {
            // Physics
            physics_update_car(game.car, CF_DELTA_TIME);
            for (int i = 0; i < OBS_COUNT; ++i)
            {
                physics_resolve_circle_collision(game.car, game.obstacles[i]);
            }
            physics_resolve_track_collision(game.car);
            physics_apply_speed_cap(game.car, 500.0f);

            // background (screen space)
            draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
            draw_quad_fill(make_aabb(V2(0, 0), WIN_WIDTH, WIN_HEIGHT), 0);
            draw_pop_color();

            // world (camera follow)
            draw_push();
            draw_translate(-game.car.x_pos, -game.car.y_pos);

            // Render
            render_world_origin();
            render_track();
            for (int i = 0; i < OBS_COUNT; ++i)
            {
                render_obstacle(game.obstacles[i]);
            }
            render_car(game.car);

            draw_pop();

            // Hud (Screen Space)
            hud_draw_stats(game.car);
            hud_draw_minimap(game.car, game.obstacles, OBS_COUNT);
        }

        app_draw_onto_screen();
    }
    destroy_app();
    return 0;
}
