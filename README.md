# Wolf3d
3D raycasting project from scratch in C.

The goal is to create a "first person shooter" style maze where the player can move around.

Graphics library used: [SDL2](https://www.libsdl.org/)

### Usage
Linux (tested on Ubuntu 18.04):
```
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev

make linux
./wolf3d maps/map
```
Map can contain numbers separated by spaces.
0 is blank space. -1 is the player's spawn point.

### Controls
WASD/Arrow keys - Move around
X, C - Change vertical Field of View
Space - Jump
Ctrl - Crouch
Q, E - Decrease/increase pixelation
Esc - Exit game

### Bonuses
- Wall collisions
- Lighting
- Multiple block types for textures (ex: Map can have blocks like 1, 2, 3... and they can all have different textures,
textures are defined in includes/wolf3d.h)
- Minimap
- FPS counter
- Pixelation filter
- Jumping and crouching
- Changing vertical Field of View
- Music

