#include <cute.h>
using namespace Cute;

int main(int argc, char *argv[])
{
    CF_Result result = make_app("Cute Test", 0, 0, 0, 1080, 600,
                                CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT, argv[0]);
    if (is_error(result))
    {
        printf("Error: %s\n", result.details);
        return -1;
    }

    struct Circle
    {
        float x_pos, y_pos;
        float radius;
        float speed;
        CF_Color color;
        float vel_x, vel_y;
    };

    // player circle
    Circle player;
    player = {
        x_pos : 0,
        y_pos : 50,
        radius : 25,
        speed : 150.0f,
        color : color_green(),
        vel_x : 0,
        vel_y : 0,
    };

    // array of balls
    Circle circles[5];

    // initialize them (postiions, colors , size)
    circles[0] = {
        200,
        50,
        20,
        100.0f,
        color_magenta(),
        0,
        0,
    };
    circles[1] = {
        -100,
        -50,
        30,
        100.0f,
        color_blue(),
        0,
        0,
    };

    circles[2] = {
        -200,
        100,
        40,
        100.0f,
        color_purple(),
        0,
        0,
    };
    circles[3] = {
        -50,
        200,
        70,
        100.0f,
        color_yellow(),
        0,
        0,
    };
    circles[4] = {
        -50,
        200,
        70,
        100.0f,
        color_orange(),
        0,
        0,
    };

    while (app_is_running())
    {
        app_update();
        // clear screen
        // --- CLEAR THE SCREEN ---
        cf_clear_color(0.0f, 0.0f, 0.0f, 0.0f); // R, G, B, A (0-1)

        draw_push_color(color_red());
        draw_circle_fill(V2(0, 0), 10);

         // task 3 : Change your speed variable to a velocity (direction + magnitude), so:

        // If KEY_A is held → velocity = -50.0f(move left)
        // If KEY_D is held → velocity = +50.0f(move right)
        // If neither → velocity = 0.0f(stop)
        // Then apply it : x_pos += velocity * CF_DELTA_TIME
        if (key_down(CF_KEY_RIGHT))
        {
            player.x_pos += player.speed * CF_DELTA_TIME;
        }
        if (key_down(CF_KEY_LEFT))
        {
            player.x_pos -= player.speed * CF_DELTA_TIME;
        }
        if (key_down(CF_KEY_UP))
        {
            player.y_pos += player.speed * CF_DELTA_TIME;
        }
        if (key_down(CF_KEY_DOWN))
        {
            player.y_pos -= player.speed * CF_DELTA_TIME;
        }

        // tasks 1: draw 4 circles, on in each cardinal direction from center, each different color
        draw_push_color(color_green());
        draw_circle_fill(V2(player.x_pos, player.y_pos), player.radius);
        draw_pop_color();

        // task 4: multiple balls
        for (int i = 0; i < 5; ++i)
        {
            draw_push_color(circles[i].color);
            draw_circle_fill(V2(circles[i].x_pos, circles[i].y_pos), circles[i].radius);
            draw_pop_color();
        }

        // task 2: drift green circle along +x each frame by speed * delta_time where speed is 50.0f
        // green_circle_y_pos -= green_circle_speed * CF_DELTA_TIME;

        // draw_push_color(color_blue());
        // draw_circle_fill(V2(-100, 0), 100);
        // draw_pop_color();

        // draw_push_color(color_yellow());
        // draw_circle_fill(V2(0, 100), 20);
        // draw_pop_color();

        // draw_push_color(color_cyan());
        // draw_circle_fill(V2(0, -100), 100);
        // draw_pop_color();

        app_draw_onto_screen();
    }

    destroy_app();
    return 0;
}