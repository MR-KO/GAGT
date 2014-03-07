#ifndef SHADERS_H
#define SHADERS_H

// Use a small offset for the shadow ray's origin, to avoid speckles
#define OFFSET 0.001

#include "types.h"

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3    ray_color(int level, vec3 ray_origin, vec3 ray_direction);

#endif
