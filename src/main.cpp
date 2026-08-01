#include <cute.h>

using namespace Cute;

// helper function to create a V2 from x and y coordinates
float check_distance(CF_V2 a, CF_V2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

int main(int argc, char *argv[])
{
    // global width and height
    int width = 1080;
    int height = 600;

    // Use OpenGL instead of Vulkan
    CF_Result result = make_app("Cute Test", 0, 0, 0, width, height,
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

    // array of balls
    Circle circles[5];
    circles[0] = {200, 50, 20, 100.0f, color_magenta(), 0, 0};
    circles[1] = {-100, -50, 30, 100.0f, color_blue(), 0, 0};
    circles[2] = {-200, 100, 40, 100.0f, color_purple(), 0, 0};
    circles[3] = {-50, 200, 70, 100.0f, color_yellow(), 0, 0};
    circles[4] = {-50, 200, 70, 100.0f, color_orange(), 0, 0};

    // Player circle
    Circle player = {
        .x_pos = 0,
        .y_pos = 50,
        .radius = 25,
        .speed = 150.0f,
        .color = color_white(),
    };

    while (app_is_running())
    {
        app_update();

        // 1. CLEAR THE SCREEN
        draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
        draw_quad_fill(make_aabb(V2(0, 0), width, height), 0);
        draw_pop_color();

        // using dual controls for movement (WASD and arrow keys)

        if (key_down(CF_KEY_A) || key_down(CF_KEY_LEFT))
        {
            player.x_pos -= player.speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_D) || key_down(CF_KEY_RIGHT))
        {
            player.x_pos += player.speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_W) || key_down(CF_KEY_UP))
        {
            player.y_pos += player.speed * CF_DELTA_TIME;
        }

        if (key_down(CF_KEY_S) || key_down(CF_KEY_DOWN))
        {
            player.y_pos -= player.speed * CF_DELTA_TIME;
        }

        // collosion detection
        for (int i = 0; i < 5; ++i){
            float dist = check_distance(CF_V2{player.x_pos, player.y_pos}, CF_V2{circles[i].x_pos, circles[i].y_pos});
            if (dist < player.radius + circles[i].radius){
                printf("Collision detected with circle %d!\n", i);
            }
        }
        // 3. DRAW EVERYTHING
        // Red dot at origin
        draw_push_color(color_red());
        draw_circle_fill(make_circle(V2(player.x_pos, player.y_pos), 10));
        draw_pop_color();

        // Other circles
        for (int i = 0; i < 5; ++i)
        {
            draw_push_color(circles[i].color);
            draw_circle_fill(make_circle(V2(circles[i].x_pos, circles[i].y_pos), circles[i].radius));
            draw_pop_color();
        }

        // Player circle
        draw_push_color(player.color);
        cf_draw_circle_fill2(V2(player.x_pos, player.y_pos), player.radius);
        draw_pop_color();

        // 4. PRESENT
        app_draw_onto_screen();
    }

    destroy_app();
    return 0;
}