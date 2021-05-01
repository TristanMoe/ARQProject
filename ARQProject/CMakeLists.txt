# CMakeList.txt : CMake project for ARQProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ARQProject")

# Add source to this project's executable.
add_executable (ARQProject "main.cpp"  "crc.h" "crc.cpp" "packets.h")

# TODO: Add tests and install targets if needed.
