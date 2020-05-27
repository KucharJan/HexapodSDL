# HexapodSDL
A c++ library expanding on the Hexapod library, adding a way to draw the joints and bones of the robot in real time.
# Dependencies
To compile the library and the demonstration program, you need the following libraries:
Eigen (available at: https://eigen.tuxfamily.org/index.php?title=Main_Page)
SDL (available at: https://www.libsdl.org/)
TTF (available at: https://www.libsdl.org/projects/SDL_ttf/)
Hexapod (available from Martin Zoula)

The Hexapod library also requires:
Dynamixel library (available from https://github.com/ROBOTIS-GIT/DynamixelSDK)
# Compile
To compile, you simply run the 'make' command in the directory.
The Makefile only expands upon the Makefile from the work of Martin Zoula; to succesfully compile, you need to combine both into one.
