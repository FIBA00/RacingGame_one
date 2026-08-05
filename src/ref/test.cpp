#include <cute.h>

using namespace Cute;

int main(int argc, char *argv[])
{
    CF_Result result = make_app(
        "My First Game",
        0, 0, 0,
        800, 600,
        CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT,
        argv[0]);

    if (is_error(result))
    {
        printf("Error: %s\n", result.details);
        return -1;
    }

    float player_x = 0.0f;
    float player_y = 0.0f;
    float speed = 200.0f;

    while (app_is_running())
    {
        app_update();
        if (key_down(CF_KEY_A))
        {
            player_x -= speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_D))
        {
            player_x += speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_W))
        {
            player_y += speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_S))
        {
            player_y -= speed * CF_DELTA_TIME;
        }
        // Background dark gray
        draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
        draw_quad_fill(make_aabb(V2(0, 0), 800, 600), 0);
        draw_pop_color();

        // Red ball
        draw_push_color(color_red());
        draw_circle_fill(make_circle(V2(player_x, player_y), 40.0f));
        draw_pop_color();

        // Blue ball
        draw_push_color(color_blue());
        draw_circle(V2(220, -100), 50.0f, 0.0f);
        draw_pop_color();

        draw_pop_color();

        app_draw_onto_screen();
    }

    destroy_app();
    return 0;
}