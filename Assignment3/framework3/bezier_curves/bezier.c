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

	printf("p[0].x = %g\n", p[0].x);
	printf("p[0].y = %g\n", p[0].y);

	// Compute the x and y values based on the bezier curve
	for (int k = 0; k < num_points; k++) {
		double binomial = binomial(num_points, k);
		double pow1 = pow(1 - u, num_points - k);
		double pow2 = pow(u, k);
		double temp = binomial * pow1 * pow2;

		*x += temp * p[0].x;
		*y += temp * p[0].y;
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
	// todo
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

