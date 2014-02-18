/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
 *
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ 18-02-2014
 * Comments ........ NULL
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

#include "bezier.h"


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

	/* Compute the x and y values based on the bezier curve */
	for (int k = 0; k < num_points; k++) {
		double binom = binomial(num_points - 1, k);
		double pow1 = pow(1.0F - u, num_points - 1 - k);
		double pow2 = pow(u, k);
		double coefficient = binom * pow1 * pow2;

		*x = *x + coefficient * p[k].x;
		*y = *y + coefficient * p[k].y;
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

	glBegin(GL_LINE_STRIP);

	for (int i = 0; i <= num_segments; i++) {
		evaluate_bezier_curve(&x, &y, p, num_points, i * ustep);
		glVertex2f(x, y);
	}

	glEnd();
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/
int intersect_cubic_bezier_curve(float *y, control_point p[], float x) {

	float eps = 1e-3;
	int num_points = 4;
	float u = 0.5;		// Start halfway through the curve
	float du = 0.25;	// And increase u with the half of u = 0.25
	float x_curve = 0;
	float y_curve = 0;

	/* Try to find an intersection using binary/exponential search... */
	for (int i = 0; i < 100; i++) {
		evaluate_bezier_curve(&x_curve, &y_curve, p, num_points, u);

		/* Check if the x value of the curve is within a small distance of the given x. */
		if (fabs(x_curve - x) <= eps) {
			*y = y_curve;
			return 1;
		}

		/* If no intersection (at all) found, quit. */
		if (u > 2.0F || u < -1.0F) {
			printf("Shit\n");
			return 0;
		}

		/* Increase or decrease u with du */
		if (x_curve > x) {
			u -= du;
		} else {
			u += du;
		}

		/* Halve du */
		du /= 2.0F;
	}

	return 0;
}

