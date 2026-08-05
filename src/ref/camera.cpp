#include <cute.h>
using namespace Cute;

int WIN_WIDTH = 1380;
int WIN_HEIGHT = 800;
const float CAR_WIDTH = 200.0f;
const float CAR_HEIGHT = 60.0f;

struct Rect
{
    float x_pos, y_pos;
    float width, height;
    CF_Color color;
    float thicknes;
    float heading;
    float speed;
    float vel_x, vel_y;
};

struct Obstacle
{
    float x_pos, y_pos;
    float radius;
    CF_Color color;
};

void draw_car(const Rect &car)
{
    draw_push();
    draw_translate(car.x_pos, car.y_pos);
    draw_rotate(-car.heading);

    CF_Aabb car_shape = cf_make_aabb_pos_w_h(V2(0, 0), CAR_WIDTH, CAR_HEIGHT);
    draw_push_color(car.color);
    draw_box_fill(car_shape, 0);
    draw_pop_color();

    // nose dot - put on local +x so forward aligns with (cos, sin)
    draw_push_color(color_white());
    draw_circle_fill(V2(CAR_WIDTH / 2.0f, 0), 8);
    draw_pop_color();

    draw_pop();
};

void draw_obstacle(const Obstacle &obs)
{
    draw_push_color(obs.color);
    draw_circle_fill(make_circle(V2(obs.x_pos, obs.y_pos), obs.radius));
    draw_pop_color();
};

void update_car_position(Rect &car)
{
    float turn_speed = 3.0f; // radians per second
    float acceleration = 400.0f;
    float friction = 0.85f;

    if (key_down(CF_KEY_A) || key_down(CF_KEY_LEFT))
    {
        car.heading += turn_speed * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_D) || key_down(CF_KEY_RIGHT))
    {
        car.heading -= turn_speed * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_W) || key_down(CF_KEY_UP))
    {
        car.vel_x += cosf(car.heading) * acceleration * CF_DELTA_TIME;
        car.vel_y += sinf(car.heading) * acceleration * CF_DELTA_TIME;
    }

    if (key_down(CF_KEY_S) || key_down(CF_KEY_DOWN))
    {
        car.vel_x -= cosf(car.heading) * acceleration * CF_DELTA_TIME;
        car.vel_y -= sinf(car.heading) * acceleration * CF_DELTA_TIME;
    }
    // friction bleeds off speed every frame even without input
    car.vel_x *= friction;
    car.vel_y *= friction;

    // velocity moves position
    car.x_pos += car.vel_x * CF_DELTA_TIME;
    car.y_pos += car.vel_y * CF_DELTA_TIME;
};

// Builds a world-space AABB centered on the car for collision math.
// This is separate from the one in draw_car - that one lives inside a rotated
// draw stack and can't be used for physics

CF_Aabb car_world_aabb(const Rect &car)
{
    return cf_make_aabb_pos_w_h(V2(car.x_pos, car.y_pos), CAR_WIDTH, CAR_HEIGHT);
};

void resolve_collision(Rect &car, const Obstacle &obs)
{
    // Wrap obstacle into CF_Circle type that CF's manifold function expects
    CF_Circle c = cf_make_circle(V2(obs.x_pos, obs.y_pos), obs.radius);
    CF_Aabb box = car_world_aabb(car);

    // cf_circle_to_aabb_manifold returns:
    // m.count     - 0 means no overlap , 1 means overlap
    // m.depths[0] - How deep the overlap is (in pixels)
    // m.n         - direction from circle center toward the AABB (push direction)
    CF_Manifold m = cf_circle_to_aabb_manifold(c, box);
    if (m.count == 0)
        return;

    // Step 1: push position out so they no longer overlap
    car.x_pos += m.n.x * m.depths[0];
    car.y_pos += m.n.y * m.depths[0];

    // Step 2: cancel the velocity component pointing INTO the obstacle.
    // dot product measures "how much of our velocity is aimed along m.n"
    // If dot < 0, we're moving against m.n = moving INTO the surface.
    // Subtracting (dot * n) removes just that inward part, leaving sideways velocity intact.
    // This is why the car slides along obstacle instead of stopping dead.

    float dot = car.vel_x * m.n.x + car.vel_y * m.n.y;
    if (dot < 0)
    {
        car.vel_x -= dot * m.n.x;
        car.vel_y -= dot * m.n.y;
    }
};

int main(int argc, char *argv[])
{

    CF_Result result = make_app("Racing Game one", 0, 0, 0, WIN_WIDTH, WIN_HEIGHT, CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT, argv[0]);
    if (is_error(result))
    {
        printf("Error: %s\n", result.details);
        return -1;
    }
    Rect car = {
        .x_pos = 200,
        .y_pos = 200,
        .color = color_red(),
        .heading = CF_PI / 2.0f,
        .speed = 400.0f,
        .vel_x = 0,
        .vel_y = 0,
    };

    // A handful of obstacles scattered around the origin.
    // Adjust positions to taste  these are in world space, same coords as car.
    const int OBS_COUNT = 5;
    Obstacle obstacles[OBS_COUNT] = {
        {200, 150, 30, color_magenta()},
        {-150, 250, 40, color_yellow()},
        {300, -100, 25, color_orange()},
        {-250, -200, 35, color_purple()},
        {100, 350, 20, color_blue()}};

    while (app_is_running())
    {
        
        app_update();

        // Update first, resolve collisions, then draw
        // so the position drawn is always post-correction (no one-frame overlap flicker)
        update_car_position(car);

          for (int i = 0; i < OBS_COUNT; ++i)
        {
            resolve_collision(car, obstacles[i]);
        }
        // background in screen space (Stay fixed, fills entire window always)
        draw_push_color(CF_Color{0.1f, 0.1f, 0.1f, 1.0f});
        draw_quad_fill(make_aabb(V2(0,0), WIN_WIDTH, WIN_HEIGHT), 0);
        draw_pop_color();
        
        // Now camera transform - everything inside this is world space
        draw_push();
        draw_translate(-car.x_pos, -car.y_pos); // camera follows car

        // central dot
        draw_push_color(color_green());
        draw_circle_fill(make_circle(V2(0, 0), 5));
        draw_pop_color();

        for (int i = 0; i < OBS_COUNT; ++i)
        {
            draw_obstacle(obstacles[i]);
        }

        // car
        draw_car(car);
        // restore to screen space
        draw_pop();
        app_draw_onto_screen();
    }
    destroy_app();
    return 0;
}
