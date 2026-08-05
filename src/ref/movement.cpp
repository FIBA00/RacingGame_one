#include <cute.h>

using namespace Cute;

// helper function to create a V2 from x and y coordinates
float check_distance(CF_V2 a, CF_V2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

struct Circle
{
    float x_pos, y_pos;
    float radius;
    float speed;
    CF_Color color;
    float vel_x, vel_y;
    float heading;
};
void update_player_position(Circle &player)
{
    float turn_speed = 3.0f; // radians per second

    if (key_down(CF_KEY_A) || key_down(CF_KEY_LEFT))
    {
        player.heading += turn_speed * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_D) || key_down(CF_KEY_RIGHT))
    {
        player.heading -= turn_speed * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_W) || key_down(CF_KEY_UP))
    {
        player.x_pos += cosf(player.heading) * player.speed * CF_DELTA_TIME;
        player.y_pos += sinf(player.heading) * player.speed * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_S) || key_down(CF_KEY_DOWN))
    {
        player.x_pos -= cosf(player.heading) * player.speed * CF_DELTA_TIME;
        player.y_pos -= sinf(player.heading) * player.speed * CF_DELTA_TIME;
    }
}
// Player circle
void draw_player_circle(const Circle &circle)
{
    draw_push_color(circle.color);
    cf_draw_circle_fill2(V2(circle.x_pos, circle.y_pos), circle.radius);
    draw_pop_color();
}

void create_circle_shape(Circle &player)
{
    // using dual controls for movement (WASD and arrow keys)
    update_player_position(player);
    draw_player_circle(player);
}

void draw_rectangle_shape()
{
    // filled rectangle
    // draw_push_color(color_yellow());
    // draw_box_fill(V2(0, 0), 200.0f, 100.0f, 0.0f);
    // draw_pop_color();

    // outline rectangle
    static float box_x = 0;
    static float box_y = 0;
    if (key_down(CF_KEY_A) || key_down(CF_KEY_LEFT))
    {
        box_x -= 200.0f * CF_DELTA_TIME;
    }
    if (key_down(CF_KEY_D) || key_down(CF_KEY_RIGHT))
    {
        box_x += 200.0f * CF_DELTA_TIME;
    }

    draw_push_color(color_red());
    draw_box(V2(box_x, box_y), 200.0f, 100.0f, 4.0f);
    draw_pop_color();
}

void draw_car(const Circle &player)
{
    float width = 120.0f;
    float height = 40.0f;

    draw_push();                                // save the current coordinate system
    draw_translate(player.x_pos, player.y_pos); // move origin to car's position
    draw_rotate(-player.heading);                // rotate around that new origin

    CF_Aabb car_shape = cf_make_aabb_pos_w_h(V2(0, 0), width, height);
    draw_push_color(color_blue());
    draw_box_fill(car_shape, 0);
    draw_pop_color();

    // nose dot - sits on local +x wherever heading = 0 currently points
    draw_push_color(color_red());
    draw_circle_fill(V2(width / 2.0f, 0), 10);
    draw_pop_color();

    draw_pop(); // restore coordinate system to original state
}

int main(int argc, char *argv[])
{
    // global width and height
    int WIN_WIDTH = 1080;
    int WIN_HEIGHT = 600;

    // Use OpenGL instead of Vulkan
    CF_Result result = make_app("Cute Test", 0, 0, 0, WIN_WIDTH, WIN_HEIGHT,
                                CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT, argv[0]);
    if (is_error(result))
    {
        printf("Error: %s\n", result.details);
        return -1;
    }

    // array of balls
    Circle circles[5];
    circles[0] = {200, 50, 20, 100.0f, color_magenta(), 0, 0};
    circles[1] = {-100, -50, 30, 100.0f, color_blue(), 0, 0};
    circles[2] = {-200, 100, 40, 100.0f, color_purple(), 0, 0};
    circles[3] = {-50, 200, 10, 100.0f, color_yellow(), 0, 0};
    circles[4] = {-50, 200, 10, 100.0f, color_orange(), 0, 0};

    // Player circle
    Circle player = {
        .x_pos = 0,
        .y_pos = 50,
        .radius = 25,
        .speed = 150.0f,
        .color = color_white(),
        .heading = CF_PI / 2.0f, // 90 degrees in radians face +y
    };

    // rectangle

    while (app_is_running())
    {
        app_update();

        // 1. CLEAR THE SCREEN
        draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
        draw_quad_fill(make_aabb(V2(0, 0), WIN_WIDTH, WIN_HEIGHT), 0);
        draw_pop_color();

        // call the circle shape function to handle player movement and drawing
        // create_circle_shape(player);
        // draw_rectangle_shape();
        update_player_position(player);
        draw_car(player);

        // // collosion detection
        for (int i = 0; i < 5; ++i){
            float dist = check_distance(CF_V2{player.x_pos, player.y_pos}, CF_V2{circles[i].x_pos, circles[i].y_pos});

            if (dist < player.radius + circles[i].radius){
                printf("Collision detected with circle %d!\n", i);
            }
        }

        // 3. DRAW EVERYTHING
        // Red dot at origin reference
        draw_push_color(color_red());
        draw_circle_fill(make_circle(V2(0, 0), 5));
        draw_pop_color();

        // Other circles
        for (int i = 0; i < 5; ++i)
        {
            draw_push_color(circles[i].color);
            draw_circle_fill(make_circle(V2(circles[i].x_pos, circles[i].y_pos), circles[i].radius));
            draw_pop_color();
        }

        // 4. PRESENT
        app_draw_onto_screen();
    }

    destroy_app();
    return 0;
}