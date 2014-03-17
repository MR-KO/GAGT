#ifndef MARCHING_TETRAHEDRA_H
#define MARCHING_TETRAHEDRA_H

#include "v3math.h"
#include "volume.h"

typedef struct {
    vec3    p[3];
    vec3    n[3];
} triangle;

int generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue);

/* Number of triangle intersections encountered, per pattern. */
extern int num_pattern_0001;
extern int num_pattern_0010;
extern int num_pattern_0100;
extern int num_pattern_1000;

/* Number of square intersections encountered, per pattern. */
extern int num_pattern_0011;
extern int num_pattern_0101;
extern int num_pattern_0110;

#endif
