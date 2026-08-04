#include <cute.h>

using namespace Cute;

const float CAR_WIDTH = 120.0f;
const float CAR_HEIGHT = 40.0f;

struct Circle
{
    float x_pos, y_pos;
    float radius;
    float speed;
    CF_Color color;
    float vel_x, vel_y;
    float heading;
};

struct Rect
{
    float x_pos, y_pos;
    float width, height;
    CF_Color color;
    float thickness;
};

void draw_circle(Circle &circles)
{
    draw_push_color(circles.color);
    draw_circle_fill(make_circle(V2(circles.x_pos, circles.y_pos), circles.radius));
    draw_pop_color();
};

void draw_rect(Rect &rects)
{
    draw_push_color(rects.color);
    draw_quad_fill(make_aabb(V2(rects.x_pos, rects.y_pos), rects.width, rects.height), 0);
    draw_pop_color();
}
void update_player_position(Circle &player)
{
    float turn_speed = 3.0f; // radians per second
    float acceleration = 400.0f;
    float friction = 0.85f;

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
        player.x_pos += cosf(player.heading) * acceleration * CF_DELTA_TIME;
        player.y_pos += sinf(player.heading) * acceleration * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_S) || key_down(CF_KEY_DOWN))
    {
        player.x_pos -= cosf(player.heading) * acceleration * CF_DELTA_TIME;
        player.y_pos -= sinf(player.heading) * acceleration * CF_DELTA_TIME;
    }
    // friction bleeds off speed every frame even without input
    player.vel_x *= friction;
    player.vel_y *= friction;

    // velocity moves position
    player.x_pos += player.vel_x * CF_DELTA_TIME;
    player.y_pos += player.vel_y * CF_DELTA_TIME;
}

void draw_car(const Circle &player)
{

    draw_push();                                // save the current coordinate system
    draw_translate(player.x_pos, player.y_pos); // move origin to car's position
    draw_rotate(-player.heading);               // rotate around that new origin

    CF_Aabb car_shape = cf_make_aabb_pos_w_h(V2(0, 0), CAR_WIDTH, CAR_HEIGHT);
    draw_push_color(color_blue());
    draw_box_fill(car_shape, 0);
    draw_pop_color();

    // nose dot - sits on local +x wherever heading = 0 currently points
    draw_push_color(color_red());
    draw_circle_fill(V2(CAR_WIDTH / 2.0f, 0), 10);
    draw_pop_color();

    draw_pop(); // restore coordinate system to original state
}
// world-space AABB for the car, ignoring roation
CF_Aabb car_world_aabb(const Circle &player)
{
    return cf_make_aabb_pos_w_h(V2(player.x_pos, player.y_pos), CAR_WIDTH, CAR_HEIGHT);
}

void resolve_car_circle_collision(Circle &player, const Circle &obstacle, const Rect &wall)
{
    CF_Circle circle_obstacles = cf_make_circle(V2(obstacle.x_pos, obstacle.y_pos), obstacle.radius);
    CF_Aabb wall_aabb = cf_make_aabb_pos_w_h(V2(wall.x_pos, wall.y_pos), wall.width, wall.height);

    CF_Aabb car_box = car_world_aabb(player);

    CF_Manifold m = cf_circle_to_aabb_manifold(circle_obstacles, car_box);
    CF_Manifold m2 = cf_circle_to_aabb_manifold(circle_obstacles, wall_aabb);
    if (m.count == 0 && m2.count == 0)
        return;

    const CF_Manifold *hit = (m.count != 0) ? &m : &m2;

    // hit.n points from the obstacle toward the car; push the car out
    player.x_pos += hit->n.x * hit->depths[0];
    player.y_pos += hit->n.y * hit->depths[0];

    // cancel velocity into the surface
    float dot = player.vel_x * hit->n.x + player.vel_y * hit->n.y;

    if (dot < 0)
    {
        player.vel_x -= dot * hit->n.x;
        player.vel_y -= dot * hit->n.y;
    }
};

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
    Rect rects[3];
    // Define a few track barriers for the car to collide with
    rects[0] = {-100, -200, 300, 50, color_red(), 2.0f};
    // rects[1] = {300, 360, 500, 50, color_green(), 2.0f};
    // rects[2] = {400, 120, 500, 50, color_blue(), 2.0f};

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

        // // collosion detection
        for (int i = 0; i < 5; ++i){
            resolve_car_circle_collision(player, circles[i], rects[0]);
        }
        draw_car(player);

        // 3. DRAW EVERYTHING
        // Red dot at origin reference
        draw_push_color(color_red());
        draw_circle_fill(make_circle(V2(0, 0), 5));
        draw_pop_color();

        // Other circles
        for (int i = 0; i < 5; ++i)
        {
            // draw_push_color(circles[i].color);
            // draw_circle_fill(make_circle(V2(circles[i].x_pos, circles[i].y_pos), circles[i].radius));
            // draw_pop_color();
            draw_circle(circles[i]);
            draw_rect(rects[i]);
        }

        // 4. PRESENT
        app_draw_onto_screen();
    }

    destroy_app();
    return 0;
}