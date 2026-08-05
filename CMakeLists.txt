cmake_minimum_required(VERSION 3.14)
project(RacingGame_one LANGUAGES C CXX)

set(CUTE_FRAMEWORK_STATIC ON)

add_subdirectory(libs/cute_framework)

# add_executable(RacingGame_one src/test.cpp)
add_executable(RacingGame_one src/main.cpp)

target_link_libraries(RacingGame_one cute)
