#include "render.h"
#include "config.h"

void render_car(const Rect &car)
{
    draw_push();
    draw_translate(car.x_pos, car.y_pos);
    draw_rotate(-car.heading);

    CF_Aabb shape = cf_make_aabb_pos_w_h(V2(0, 0), CAR_WIDTH, CAR_HEIGHT);
    draw_push_color(car.color);
    draw_box_fill(shape, 0);
    draw_pop_color();

    draw_push_color(color_white());
    draw_circle_fill(V2(CAR_WIDTH / 2.0f, 0), 8);
    draw_pop_color();

    draw_pop();
};


void render_obstacle(const Obstacle &obs) {
    draw_push_color(obs.color);
    draw_circle_fill(make_circle(V2(obs.x_pos, obs.y_pos), obs.radius));
    draw_pop_color();
};


void render_track() {
    draw_push_color(CF_Color{0.3f, 0.3f, 0.3f, 1.0f});
    draw_box(V2(0,0), TRACK_WIDTH, TRACK_HEIGHT, 20.0f);
    draw_pop_color();
};

void render_world_origin() {
    draw_push_color(color_green());
    draw_circle_fill(make_circle(V2(0,0), 15));
    draw_pop_color();
};
