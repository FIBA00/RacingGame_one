cmake_minimum_required(VERSION 3.14)
project(RacingGame_one LANGUAGES C CXX)

set(CUTE_FRAMEWORK_STATIC ON)
# set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs/cute_framework)

# add_executable(RacingGame_one src/test.cpp)
add_executable(RacingGame_one 
src/main.cpp
src/engine/physics.cpp
src/engine/render.cpp
src/hud.cpp
src/loading.cpp
)
target_include_directories(RacingGame_one PRIVATE src)
target_link_libraries(RacingGame_one cute)
