/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ Tue March 18
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

/* Number of triangle intersections encountered, per pattern. */
int num_pattern_0001 = 0;
int num_pattern_0010 = 0;
int num_pattern_0100 = 0;
int num_pattern_1000 = 0;

/* Number of square intersections encountered, per pattern. */
int num_pattern_0011 = 0;
int num_pattern_0101 = 0;
int num_pattern_0110 = 0;

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */
static vec3 interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2,
	unsigned char v1, unsigned char v2) {

	/* Initially, simply return the midpoint between p1 and p2.
	   So no real interpolation is done yet */
	// return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));

	/* Calculate the intersection point using lineair interpolation of the line p1 - p2. */
	float t = (isovalue - v1) / (v2 - v1);

	/* Then use that t value to calculate the interpolated point between p1 and p2. */
	vec3 p = v3_add(p1, v3_multiply(v3_subtract(p2, p1), t));
	return p;
}

// From: stackoverflow.com/questions/1516926
static void calc_triangle_normal(triangle *triangles) {
	vec3 normal = v3_normalize(v3_crossprod(v3_subtract(triangles->p[1], triangles->p[0]),
		v3_subtract(triangles->p[2], triangles->p[0])));

	triangles->n[0] = triangles->n[1] = triangles->n[2] = normal;
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.
   Store the resulting triangles in the "triangles" array.
   Return the number of triangles created (either 0, 1, or 2).
   Note: the output array "triangles" should have space for at least
   2 triangles.
*/
static int generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue,
cell c, int v0, int v1, int v2, int v3) {

	/* Count how many vertices are greater than the isovalue, or less than. */
	int num_greater = 0;
	int num_lesser = 0;
	int pattern = 0x0;

	c.value[v0] > isovalue ? num_greater++, pattern = pattern | 0x1 	: num_lesser++;
	c.value[v1] > isovalue ? num_greater++, pattern = pattern | 0x10	: num_lesser++;
	c.value[v2] > isovalue ? num_greater++, pattern = pattern | 0x100	: num_lesser++;
	c.value[v3] > isovalue ? num_greater++, pattern = pattern | 0x1000	: num_lesser++;

	/* Examine all possibilities and create the appropriate amount of triangles. */
	if (num_greater == 0 || num_lesser == 0) {
		return 0;
	}

	/* Examine the possibilities where there is only 1 intersecting triangle. */
	if (num_greater == 3 || num_lesser == 3) {
		/* Examine each pattern and calculate the intersecting vertices for the triangle. */
		if (pattern == 0x0001 || pattern == 0x1110) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v3],
				c.value[v0], c.value[v3]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v0], c.p[v1],
				c.value[v0], c.value[v1]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v0], c.p[v2],
				c.value[v0], c.value[v2]);

			num_pattern_0001++;
		} else if (pattern == 0x0010 || pattern == 0x1101) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1],
				c.value[v0], c.value[v1]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2],
				c.value[v1], c.value[v2]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3],
				c.value[v1], c.value[v3]);

			num_pattern_0010++;
		} else if (pattern == 0x0100 || pattern == 0x1011) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v1], c.p[v2],
				c.value[v1], c.value[v2]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v2], c.p[v3],
				c.value[v2], c.value[v3]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v0], c.p[v2],
				c.value[v0], c.value[v2]);

			num_pattern_0100++;
		} else if (pattern == 0x1000 || pattern == 0x0111) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v2], c.p[v3],
				c.value[v2], c.value[v3]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v0], c.p[v3],
				c.value[v0], c.value[v3]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3],
				c.value[v1], c.value[v3]);

			num_pattern_1000++;
		}

		calc_triangle_normal(triangles);
		return 1;
	}

	/* Examine the possibilities where there is an intersecting square. */
	if (num_greater == 2 && num_lesser == 2) {
		/* Examine each pattern and divide it diagonally into 2 triangles. */
		if (pattern == 0x0011 || pattern == 0x1100) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v3],
				c.value[v0], c.value[v3]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v0], c.p[v2],
				c.value[v0], c.value[v2]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v3],
				c.value[v1], c.value[v3]);

			(triangles + 1)->p[0] = triangles->p[0];
			(triangles + 1)->p[1] = triangles->p[1];
			(triangles + 1)->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v2],
				c.value[v1], c.value[v2]);

			num_pattern_0011++;
		} else if (pattern == 0x0101 || pattern == 0x1010) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v3],
				c.value[v0], c.value[v3]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v2],
				c.value[v1], c.value[v2]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3],
				c.value[v2], c.value[v3]);

			(triangles + 1)->p[0] = triangles->p[0];
			(triangles + 1)->p[1] = triangles->p[1];
			(triangles + 1)->p[2] = interpolate_points(isovalue, c.p[v0], c.p[v1],
				c.value[v0], c.value[v1]);

			num_pattern_0101++;
		} else if (pattern == 0x0110 || pattern == 0x1001) {
			triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v2],
				c.value[v0], c.value[v2]);
			triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v3],
				c.value[v1], c.value[v3]);
			triangles->p[2] = interpolate_points(isovalue, c.p[v0], c.p[v1],
				c.value[v0], c.value[v1]);

			(triangles + 1)->p[0] = triangles->p[0];
			(triangles + 1)->p[1] = triangles->p[1];
			(triangles + 1)->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3],
				c.value[v2], c.value[v3]);

			num_pattern_0110++;
		}

		calc_triangle_normal(triangles);
		calc_triangle_normal(triangles + 1);
		return 2;
	}

	return 0;
}

/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
*/

int generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue) {
	int triangles_created = 0;

	/* Generate all 6 possible tetrahedra in a cell. */
	triangles_created += generate_tetrahedron_triangles(triangles,
		isovalue, c, 0, 7, 3, 1);

	triangles_created += generate_tetrahedron_triangles(triangles + triangles_created,
		isovalue, c, 0, 2, 6, 7);

	triangles_created += generate_tetrahedron_triangles(triangles + triangles_created,
		isovalue, c, 0, 7, 5, 1);

	triangles_created += generate_tetrahedron_triangles(triangles + triangles_created,
		isovalue, c, 0, 2, 7, 3);

	triangles_created += generate_tetrahedron_triangles(triangles + triangles_created,
		isovalue, c, 0, 4, 7, 5);

	triangles_created += generate_tetrahedron_triangles(triangles + triangles_created,
		isovalue, c, 0, 4, 6, 7);

	return triangles_created;
}


