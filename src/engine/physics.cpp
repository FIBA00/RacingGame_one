#include "physics.h"
#include "config.h"
#include <cmath>

void physics_update_car(Rect &car, float delta_time)
{
    float turn_speed = CF_PI / 4.0f;
    float friction = 0.92f;
    float max_rpm = car.rpm;
    float idle_rpm = 500.0f;
    float gear_accel[] = {0, 1200.0f, 900.0f, 600.0f, 400.0f, 250.0f}; // index 0 unused

    if (key_down(CF_KEY_A) || key_down(CF_KEY_LEFT))
    {
        car.heading += turn_speed * delta_time;
    }
    if (key_down(CF_KEY_D) || key_down(CF_KEY_RIGHT))
    {
        car.heading -= turn_speed * delta_time;
    }

    // change gear based speed ag
    float speed_mag = sqrtf(car.vel_x * car.vel_x + car.vel_y * car.vel_y);

    if (speed_mag < 50.0f)
    {
        car.gear = 1;
    }
    else if (speed_mag < 120.0f)
    {
        car.gear = 2;
    }
    else if (speed_mag < 200.0f)
    {
        car.gear = 3;
    }
    else if (speed_mag < 280.0f)
    {
        car.gear = 4;
    }
    else
    {
        car.gear = 5;
    }

    // change the gear
    float accel = gear_accel[car.gear];

    if (key_down(CF_KEY_W) || key_down(CF_KEY_UP))
    {
        car.vel_x += cosf(car.heading) * accel * delta_time;
        car.vel_y += sinf(car.heading) * accel * delta_time;
        car.rpm += 2000.0f * delta_time;
    }

    if (key_down(CF_KEY_S) || key_down(CF_KEY_DOWN))
    {
        float fwd_vel = car.vel_x * cosf(car.heading) + car.vel_y * sinf(car.heading);
        if (fwd_vel > 0)
        {
            car.vel_x -= cosf(car.heading) * fwd_vel * 3.0f * delta_time;
            car.vel_y -= sinf(car.heading) * fwd_vel * 3.0f * delta_time;
        }
        car.rpm -= 1500.0f * delta_time;
    }
    // RPM ticks down when not throttling , bottoms at idle
    if (!key_down(CF_KEY_W) && !key_down(CF_KEY_UP))
    {
        car.rpm -= 1000.0f * CF_DELTA_TIME;
    }
    car.rpm = fmaxf(idle_rpm, fminf(car.rpm, max_rpm)); // clamp to [idle, max]

    // friction bleeds off speed every frame even without input
    car.vel_x *= friction;
    car.vel_y *= friction;

    // velocity moves position
    car.x_pos += car.vel_x * CF_DELTA_TIME;
    car.y_pos += car.vel_y * CF_DELTA_TIME;
};

void physics_apply_speed_cap(Rect &car, float max_speed)
{
    float magnitude = sqrtf(car.vel_x * car.vel_x + car.vel_y * car.vel_y);
    if (magnitude > max_speed)
    {
        car.vel_x = (car.vel_x / magnitude) * max_speed;
        car.vel_y = (car.vel_y / magnitude) * max_speed;
    }
};

void physics_resolve_circle_collision(Rect &car, const Obstacle &obs)
{
    // Wrap obstacle into CF_Circle type that CF's manifold function expects
    CF_Circle c = cf_make_circle(V2(obs.x_pos, obs.y_pos), obs.radius);
    CF_Aabb box = cf_make_aabb_pos_w_h(V2(car.x_pos, car.y_pos), CAR_WIDTH, CAR_HEIGHT);
    CF_Manifold m = cf_circle_to_aabb_manifold(c, box);

    if (m.count == 0)
        return;

    // cf_circle_to_aabb_manifold returns:
    // m.count     - 0 means no overlap , 1 means overlap
    // m.depths[0] - How deep the overlap is (in pixels)
    // m.n         - direction from circle center toward the AABB (push direction)

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

void physics_resolve_track_collision(Rect &car)
{
    CF_Aabb track = cf_make_aabb_pos_w_h(V2(0, 0), TRACK_WIDTH, TRACK_HEIGHT);
    CF_Aabb car_box = cf_make_aabb_pos_w_h(V2(car.x_pos, car.y_pos), CAR_WIDTH, CAR_HEIGHT);
    CF_Manifold m = cf_aabb_to_aabb_manifold(car_box, track);

    if (m.count == 0)
        return;

    car.x_pos += m.n.x * m.depths[0];
    car.y_pos += m.n.y * m.depths[0];

    float dot = car.vel_x * m.n.x + car.vel_y * m.n.y;
    if (dot < 0)
    {
        car.vel_x -= dot * m.n.x;
        car.vel_y -= dot * m.n.y;
    }
};
