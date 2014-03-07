/* Computer Graphics and Game Technology, Assignment Ray-tracing
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include <stdio.h>
#include "intersection.h"
#include "v3math.h"
#include "constants.h"
#include "scene.h"
#include "bvh.h"

#include "stack.h"

// Used to create the stack with a size dependent on the max depth of the bvh tree.
extern int bvh_tree_max_depth;

// A few counters for gathering statistics on the number and types
// of ray shot

// The total number of rays
int num_rays_shot = 0;

// Number of shadow rays
int num_shadow_rays_shot = 0;

// Number of triangles tested for intersection
int num_triangles_tested = 0;

// Number of bounding boxes tested for intersection
int num_bboxes_tested = 0;

// Forward declarations

static int  find_first_intersected_bvh_triangle(intersection_point* ip,
				vec3 ray_origin, vec3 ray_direction);

// Checks if the given triangle is intersected by ray with given
// origin and direction.
//
// Returns 1 if there is an intersection, or 0 otherwise.
//
// When an intersection is found the fields of 'ip' will be filled in
// with the relevant values.
//
// Note: this routine does NOT return an intersection for triangles
// whose back side faces the ray (by definition a triangle normal
// points to the triangle's front side).
// I.e. we do back-face culling here ...
//
// Code based on Moller & Trumbore, 1997, "Fast, minimum storage
// ray/triangle intersection"

static int
ray_intersects_triangle(intersection_point* ip, triangle tri,
	vec3 ray_origin, vec3 ray_direction)
{
	vec3    edge1, edge2;
	vec3    tvec, pvec, qvec;
	double  det, inv_det;
	double  t, u, v;        // u, v are barycentric coordinates
	// t is ray parameter

	num_triangles_tested++;

	edge1 = v3_subtract(scene_vertices[tri.v[1]], scene_vertices[tri.v[0]]);
	edge2 = v3_subtract(scene_vertices[tri.v[2]], scene_vertices[tri.v[0]]);

	pvec = v3_crossprod(ray_direction, edge2);

	det = v3_dotprod(edge1, pvec);

	if (det < 1.0e-6)
		return 0;

	tvec = v3_subtract(ray_origin, scene_vertices[tri.v[0]]);

	u = v3_dotprod(tvec, pvec);
	if (u < 0.0 || u > det)
		return 0;

	qvec = v3_crossprod(tvec, edge1);

	v = v3_dotprod(ray_direction, qvec);
	if (v < 0.0 || u+v > det)
		return 0;

	t = v3_dotprod(edge2, qvec);

	if (t < 0.0)
		return 0;

	inv_det = 1.0 / det;
	t *= inv_det;
	u *= inv_det;
	v *= inv_det;

	// We have a triangle intersection!
	// Return the relevant intersection values.

	// Compute the actual intersection point
	ip->t = t;
	ip->p = v3_add(ray_origin, v3_multiply(ray_direction, t));

	// Compute an interpolated normal for this intersection point, i.e.
	// we use the barycentric coordinates as weights for the vertex normals
	ip->n = v3_normalize(v3_add(
		v3_add(
			v3_multiply(tri.vn[0], 1.0-u-v),
			v3_multiply(tri.vn[1], u)
		),
		v3_multiply(tri.vn[2], v)));

	ip->i = v3_normalize(v3_negate(ray_direction));
	ip->material = tri.material;

	return 1;
}

// Check if the given sphere is intersected by the given ray.
// See Shirley et.al., section 10.3.1
// Returns 1 if there is an intersection (and sets the appropriate
// fields of ip), or 0 otherwise.
static int
ray_intersects_sphere(intersection_point* ip, sphere sph,
	vec3 ray_origin, vec3 ray_direction)
{
	float   A, B, C, D;
	vec3    diff;
	float   t_hit;

	A = v3_dotprod(ray_direction, ray_direction);

	diff = v3_subtract(ray_origin, sph.center);
	B = 2.0 * v3_dotprod(diff, ray_direction);
	C = v3_dotprod(diff, diff) - sph.radius * sph.radius;

	D = B*B - 4*A*C;

	if (D < 0.0)
		return 0;

	D = sqrt(D);

	// We're only interested in the first hit, i.e. the one with
	// the smallest t_hit, so we check -B-D first, followed by -B+D

	t_hit = (-B - D)/(2*A);

	if (t_hit < 0.0)
	{
		t_hit = (-B + D)/(2*A);
		if (t_hit < 0.0)
			return 0;
	}

	ip->t = t_hit;
	ip->p = v3_add(ray_origin, v3_multiply(ray_direction, t_hit));
	ip->n = v3_normalize(v3_subtract(ip->p, sph.center));
	ip->i = v3_normalize(v3_negate(ray_direction));
	ip->material = sph.material;

	return 1;
}

// Checks for an intersection of the given ray with the triangles
// stored in the BVH.
//
// Returns 1 if there is an intersection. The fields of 'ip' will be
// set to the relevant values. The intersection returned
// will be the one closest to the ray origin.
//
// Returns 0 if there are no intersections

static int
find_first_intersected_bvh_triangle(intersection_point* ip,
	vec3 ray_origin, vec3 ray_direction) {

	float tmin;
	float tmax;

	float t0 = 0;
	float t1 = C_INFINITY;
	float tmin_left = 0;
	float tmax_left = C_INFINITY;
	float tmin_right = 0;
	float tmax_right = C_INFINITY;

	int found_left = 0;
	int found_right = 0;
	int have_hit = 0;

	bvh_node *current = bvh_root;
	bvh_node *left;
	bvh_node *right;

	intersection_point ip2;
	float t_nearest = C_INFINITY;

	int num_triangles = 0;
	triangle *triangles = NULL;

	int num_loops = 0;

	// Initialize a stack for the bvh traversal, of size bvh_tree_max_depth.
	// The multiplication by 2 is just to ensure that the stack is always large enough,
	// since resizing is currently NOT supported (or needed).
	stack_t *stack = stack_init(bvh_tree_max_depth * 2);

	// First check against spheres in the scene
	for (int s = 0; s < scene_num_spheres; s++) {
		// We need a second set of p and n variables, as there's the
		// possibility that we'll overwrite a closer intersection already
		// found
		if (ray_intersects_sphere(&ip2, scene_spheres[s], ray_origin, ray_direction)) {
			if (ip2.t < t_nearest) {
				*ip = ip2;
				t_nearest = ip2.t;
				have_hit = 1;
			}
		}
	}

	// If the stack could not be initialized, we could only check for spheres...
	if (stack == NULL) {
		fprintf(stderr, "\nFailed to initialize the stack required for bboxes/triangles intersection!\n");
		return have_hit;
	}

	// Any ray should intersect the bvh root node, or possibly a sphere
	if (!bbox_intersect(&tmin, &tmax, bvh_root->bbox, ray_origin, ray_direction, t0, t1)) {
		stack_destroy(stack);
		return have_hit;
	}

	// t_nearest = tmax;

	// Else, search for the first intersecting triangle by traversing the
	// bvh tree using bounding boxes
	while(1) {
		// Reset variables and adjust the t0, t1 and tmin, tmax values
		// t0 = tmin;
		// t1 = tmax;

		tmin_left = tmin;
		tmax_left = tmax;
		tmin_right = tmin;
		tmax_right = tmax;

		found_left = 0;
		found_right = 0;

		num_loops++;

		// If the current node is a leaf node, check its triangles
		if (current->is_leaf) {
			// Check for closest intersecting triangle in the leaf node
			num_triangles = leaf_node_num_triangles(current);
			triangles = leaf_node_triangles(current);

			for (int i = 0; i < num_triangles; i++) {
				if (ray_intersects_triangle(&ip2, triangles[i], ray_origin, ray_direction)) {
					if (ip2.t < t_nearest) {
						*ip = ip2;
						t_nearest = ip2.t;
						have_hit = 1;
					}
				}
			}

			stack_destroy(stack);
			return have_hit;
		}

		// Else, we check the left and right child nodes
		left = inner_node_left_child(current);
		right = inner_node_right_child(current);

		if (bbox_intersect(&tmin_left, &tmax_left, left->bbox, ray_origin, ray_direction, t0, t1)) {
			// fprintf(stderr, "Found left intersection!\n");
			found_left = 1;
		}

		if (bbox_intersect(&tmin_right, &tmax_right, right->bbox, ray_origin, ray_direction, t0, t1)) {
			// fprintf(stderr, "Found right intersection!\n");
			found_right = 1;
		}

		// 4 Options are possible:
		// Only found an intersection in the left box, and not the right box
		if (found_left && !found_right) {
			current = left;
			continue;
		}

		// Only found an intersection in the right box, and not the left box
		if (found_right && !found_left) {
			current = right;
			continue;
		}

		// Found no intersection at all, either quit or use the backup
		if (!found_left && !found_right) {
			// fprintf(stderr, "No intersection found!\n");
			current = stack_pop(stack);

			if (current == NULL) {
				stack_destroy(stack);
				return have_hit;
			}
		}

		// Found an intersection in both the left and right box, traverse the closest bbox
		if (found_left && found_right) {
			// Push the non-chosen node on the stack, so we could try it later
			if (tmin_left < tmin_right) {
				current = left;
				tmin = tmin_left;
				tmax = tmax_left;

				if (stack_push(stack, right) != STACK_SUCCESS) {
					fprintf(stderr, "Whoops, error occurred!\n");
				} else {
					// fprintf(stderr, "Pushed right node on the stack!\n");
				}
			} else {
				current = right;
				tmin = tmin_left;
				tmax = tmax_left;

				if (stack_push(stack, left) != STACK_SUCCESS) {
					fprintf(stderr, "Whoops, error occurred!\n");
				} else {
					// fprintf(stderr, "Pushed left node on the stack!\n");
				}
			}
		}
	}

	stack_destroy(stack);
	fprintf(stderr, "DERP!\n");
	return have_hit;
}

// Returns the nearest hit of the given ray with objects in the scene
// (either a sphere or a triangle).
//
// Returns 1 and sets the intersection point values if there
// is an intersection, returns 0 otherwise.
int
find_first_intersection(intersection_point *ip, vec3 ray_origin, vec3 ray_direction)
{
	int     have_hit;
	float   t_nearest;
	intersection_point  ip2;

	num_rays_shot++;

	// We have found no hit yet
	t_nearest = C_INFINITY;
	have_hit = 0;

	// First check against spheres in the scene
	for (int s = 0; s < scene_num_spheres; s++)
	{
		// We need a second set of p and n variables, as there's the
		// possibility that we'll overwrite a closer intersection already
		// found
		if (ray_intersects_sphere(&ip2, scene_spheres[s], ray_origin, ray_direction))
		{
			if (ip2.t < t_nearest)
			{
				*ip = ip2;
				t_nearest = ip2.t;
				have_hit = 1;
			}
		}
	}

	// Then check against triangles in the scene

	if (use_bvh)
	{
		// Use the BVH to speed up intersection testing
		if (find_first_intersected_bvh_triangle(&ip2, ray_origin, ray_direction))
		{
			if (ip2.t < t_nearest)
			{
				*ip = ip2;
				t_nearest = ip2.t;
				have_hit = 1;
			}
		}
	}
	else
	{
		// Simply iterate over all the triangles in the scene and check for intersection
		for (int t = 0; t < scene_num_triangles; t++)
		{
			if (ray_intersects_triangle(&ip2, scene_triangles[t], ray_origin, ray_direction))
			{
				if (ip2.t < t_nearest)
				{
					*ip = ip2;
					t_nearest = ip2.t;
					have_hit = 1;
				}
			}
		}
	}

	return have_hit;
}

// Optimized routine for tracing a shadow ray.
//
// This routine doesn't return the nearest intersection, but simply
// checks if there is any intersection.
int
shadow_check(vec3 ray_origin, vec3 ray_direction)
{
	intersection_point  ip;

	num_rays_shot++;
	num_shadow_rays_shot++;

	for (int s = 0; s < scene_num_spheres; s++)
	{
		if (ray_intersects_sphere(&ip, scene_spheres[s], ray_origin, ray_direction))
			return 1;
	}

	if (use_bvh)
	{
		// Use the BVH for speedy intersection testing
		if (find_first_intersected_bvh_triangle(&ip, ray_origin, ray_direction))
			return 1;
	}
	else
	{
		// Simply iterate over all the triangles in the scene and check for intersection
		for (int t = 0; t < scene_num_triangles; t++)
		{
			if (ray_intersects_triangle(&ip, scene_triangles[t], ray_origin, ray_direction))
				return 1;
		}
	}

	return 0;
}

