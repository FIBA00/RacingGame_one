cmake -B build .

cmake --build build -j$(nproc)

cd build && ./RacingGame_one

cd ../