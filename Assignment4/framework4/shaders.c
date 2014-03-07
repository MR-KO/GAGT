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
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip) {
	return v3_create(1, 0, 0);
}

vec3
shade_matte(intersection_point ip) {
	// There is always ambient light
	vec3 color = v3_create(scene_ambient_light, scene_ambient_light, scene_ambient_light);
	vec3 light;
	int shadow = 0;

	// The total color is the sum of all scene lights interacting with the object
	for (int i = 0; i < scene_num_lights; i++) {
		// Calculate where the light came from
		light = v3_normalize(v3_subtract(scene_lights[i].position, ip.p));

		// Calculate the dot product, which specifies the significance of the light
		float dp = v3_dotprod(ip.n, light);

		// Check for shadow rays, and use a small offset to avoid speckles
		shadow = shadow_check(v3_add(ip.p, v3_create(0.001, 0.001, 0.001)), light);

		if (!shadow) {
			// Add the intensity multiplied by fmax(0, dp) to the color if its not a shadow ray
			color = v3_add(color, v3_multiply(v3_create(scene_lights[i].intensity,
				scene_lights[i].intensity, scene_lights[i].intensity), fmax(0, dp)));
		}
	}

	return color;
}

vec3
shade_blinn_phong(intersection_point ip) {
	// Define constants
	float kd = 0.8;
	float ks = 0.5;
	float alpha = 50;
	vec3 cd = v3_create(1, 0, 0);
	vec3 cs = v3_create(1, 1, 1);

	// Use a small offset for the shadow ray's origin, to avoid speckles
	vec3 offset = v3_create(0.001, 0.001, 0.001);
	vec3 shadow_origin = v3_add(ip.p, offset);

	vec3 h;
	vec3 light;
	float light_sum_diffuse = 0;
	float light_sum_specular = 0;

	// The total color is the sum of all scene lights interacting with the object
	for (int i = 0; i < scene_num_lights; i++) {
		// Calculate where the light came from
		light = v3_normalize(v3_subtract(scene_lights[i].position, ip.p));

		// Calculate the halfway vector
		h = v3_normalize(v3_add(scene_camera_position, light));

		// Only add the light to the colors object if it is no shadow ray
		if (!shadow_check(shadow_origin, light)) {
			// The sum of diffuse light = sum(Ii * max(0, n . li))
			light_sum_diffuse += scene_lights[i].intensity * fmax(0, v3_dotprod(ip.n, light));

			// The sum of specular light = sum(Ii * (n . h)^alpha)
			light_sum_specular += scene_lights[i].intensity * pow(v3_dotprod(ip.n, h), alpha);
		}
	}

	// The color is defined as the following:
	// cf = cd * (Ia + kd * light_sum_diffuse +
	// 		cs * ks * light_sum_specular
	vec3 color = v3_add(v3_multiply(cd, scene_ambient_light + kd * light_sum_diffuse),
		v3_multiply(cs, ks * light_sum_specular));
	return color;
}

vec3
shade_reflection(intersection_point ip)
{
	return v3_create(1, 0, 0);
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
	case 0:
	  return shade_constant(ip);
	case 1:
	  return shade_matte(ip);
	case 2:
	  return shade_blinn_phong(ip);
	case 3:
	  return shade_reflection(ip);
	default:
	  return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
	intersection_point ip;

	// If this ray has been reflected too many times, simply
	// return the background color.
	if (level >= 3)
		return scene_background_color;

	// Check if the ray intersects anything in the scene
	if (find_first_intersection(&ip, ray_origin, ray_direction))
	{
		// Shade the found intersection point
		ip.ray_level = level;
		return shade(ip);
	}

	// Nothing was hit, return background color
	return scene_background_color;
}
