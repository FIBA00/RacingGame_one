# rebuild the build and go back to cmake from ninja 
# cmake -G "Unix Makefiles" . 

# cmake -B build .

# cmake --build build -j$(nproc)

# cd build && ./RacingGame_one

# cd ../

#------------------------NINJA-----------------------
# mkdir build & 
cd build
# cmake -G "Ninja" ..
ninja
./RacingGame_one