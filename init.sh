mkdir cmake
cd cmake


cmake ..
cmake -S . -B -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest
