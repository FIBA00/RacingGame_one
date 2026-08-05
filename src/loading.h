#ifndef LOADING_H
#define LOADING_H

#include <cute.h>
using namespace Cute;

struct LoadingState {
    float elapsed; //time since loading started
    float duration; // total time to show loading screen
    bool done; 
};

void loading_init(LoadingState &state, float duration);
void loading_update(LoadingState &state, float delta_time);
void loading_draw(const LoadingState &state);

#endif