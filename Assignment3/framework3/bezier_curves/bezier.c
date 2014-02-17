/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
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
#include "bezier.h"
#include <stdio.h>


/* Returns n! , if possible. 1 is returned for n <= 0. */
double factoriald(int n) {
	if (n <= 0) {
		return 1;
	}

	double result = n;

	while (--n > 1) {
		result *= n;
	}

	return result;
}

/* Returns the binomial coefficient of (n above k) */
double binomial(int n, int k) {
	return factoriald(n) / (factoriald(k) * factoriald(n - k));
}

/* Given a Bezier curve defined by the 'num_points' control points
 * in 'p' compute the position of the point on the curve for parameter
 * value 'u'.
 *
 * Return the x and y values of the point by setting *x and *y,
 * respectively.
 */
void evaluate_bezier_curve(float *x, float *y, control_point p[], int num_points, float u) {
	*x = 0.0;
	*y = 0.0;

	// Compute the x and y values based on the bezier curve
	for (int k = 0; k < num_points; k++) {
		double binom = binomial(num_points - 1, k);
		double pow1 = pow(1.0F - u, num_points - 1 - k);
		double pow2 = pow(u, k);
		double temp = binom * pow1 * pow2;

		*x = *x + temp * p[k].x;
		*y = *y + temp * p[k].y;

		// printf("\tk = %2d, binom = %8g, pow1 = %8g, pow2 = %8g, temp = %8g\n",
		// 	k, binom, pow1, pow2, temp);
		// printf("\t\tx = %8g, y = %8g\n", *x, *y);
	}
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 * Draw the line segments you compute directly on the screen
 * as a single GL_LINE_STRIP. This is as simple as using
 *
 *      glBegin(GL_LINE_STRIP);
 *      glVertex2f(..., ...);
 *      ...
 *      glEnd();
 *
 * DO NOT SET ANY COLOR!
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 */

void draw_bezier_curve(int num_segments, control_point p[], int num_points) {
	float ustep = 1.0 / num_segments;
	float x = 0.0;
	float y = 0.0;

	// printf("num_points = %2d, num_segments = %2d, ustep = %g\n",
		// num_points, num_segments, ustep);

	for (int i = 0; i < num_points; i++) {
		// printf("Control point %2d: (%8g, %8g)\n", i, p[i].x, p[i].y);
	}

	glBegin(GL_LINE_STRIP);
	// glVertex2f(p[0].x, p[0].y);

	for (int i = 0; i <= num_segments; i++) {
		evaluate_bezier_curve(&x, &y, p, num_points, i * ustep);
		glVertex2f(x, y);
	}

	// glVertex2f(p[num_points - 1].x, p[num_points - 1].y);
	glEnd();
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/

int
intersect_cubic_bezier_curve(float *y, control_point p[], float x)
{
	return 0;
}

