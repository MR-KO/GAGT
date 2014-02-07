/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
 * Created by ...... Paul Melis
 *
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ 07-02-2014
 * Comments ........ NULL
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "trirast.h"


/* Lines through the points P0 (x0, y0), P1 (x1, y1) and P2 (x2, y2) */
inline float f_01(int x, int y, float x0, float y0, float x1, float y1) {
	return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

inline float f_12(int x, int y, float x1, float y1, float x2, float y2) {
	return (y1 - y2) * x + (x2 - x1) * y + x1 * y2 - x2 * y1;
}

inline float f_20(int x, int y, float x0, float y0, float x2, float y2) {
	return (y2 - y0) * x + (x0 - x2) * y + x2 * y0 - x0 * y2;
}


/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */
void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
	byte r, byte g, byte b) {

	float alpha = 0, beta = 0, gamma = 0;

	/*
		Define non-changing variables. This will increase the setup time, but allows
		for more readable (and probably faster) code.
	*/
	const float f_alpha = f_12(x0, y0, x1, y1, x2, y2);
	const float f_beta = f_20(x1, y1, x0, y0, x2, y2);
	const float f_gamma = f_01(x2, y2, x0, y0, x1, y1);

	/* These values determine the bounding box */
	const int xmin = (int) floor(getMin(x0, x1, x2));
	const int xmax = (int) ceil(getMax(x0, x1, x2));

	const int ymin = (int) floor(getMin(y0, y1, y2));
	const int ymax = (int) ceil(getMax(y0, y1, y2));

	/* Loop over all pixels in the bounding box */
	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			/* Determine the barrycentric (alpha, beta, gamma) coordinates */
			alpha = f_12(x, y, x1, y1, x2, y2) / f_alpha;
			beta = f_20(x, y, x0, y0, x2, y2) / f_beta;
			gamma = f_01(x, y, x0, y0, x1, y1) / f_gamma;
			// alpha = 1 - beta - gamma;

			/* Only draw the pixel if it is inside the triangle */
			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				/*
					And if it is on the edge of the triangle, use the offscreen
					point to determine if the pixel should be drawn. The pixel
					on the same side of the edge as the offscreen point will be
					drawn.
				*/
				if (((alpha > 0) || (f_alpha * f_12(X_OFF, Y_OFF, x1, y1, x2, y2) > 0)) &&
					((beta > 0) || (f_beta * f_20(X_OFF, Y_OFF, x0, y0, x2, y2) > 0)) &&
					((gamma > 0) || (f_gamma * f_01(X_OFF, Y_OFF, x0, y0, x1, y1) > 0))) {

					PutPixel(x, y, r, g, b);
				} else if (
					/*
						In case the shared edge goes through the offscreen point,
						use a backup offscreen point.
					*/
					((alpha > 0) || (f_alpha * f_12(X_OFF_BACKUP, Y_OFF_BACKUP, x1, y1, x2, y2) > 0)) &&
					((beta > 0) || (f_beta * f_20(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x2, y2) > 0)) &&
					((gamma > 0) || (f_gamma * f_01(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x1, y1) > 0))) {

					PutPixel(x, y, r, g, b);
				}
			}
		}
	}
}

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 * This version uses various optimization tweaks, like incremental calculations.
 */
void draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
	byte r, byte g, byte b) {

	/*
		Define non-changing variables. This will increase the setup time, but allows
		for more readable (and probably faster) code.
	*/
	const int xmin = (int) floor(getMin(x0, x1, x2));
	const int xmax = (int) ceil(getMax(x0, x1, x2));

	const int ymin = (int) floor(getMin(y0, y1, y2));
	const int ymax = (int) ceil(getMax(y0, y1, y2));

	const float f_alpha = f_12(x0, y0, x1, y1, x2, y2);
	const float f_beta = f_20(x1, y1, x0, y0, x2, y2);
	const float f_gamma = f_01(x2, y2, x0, y0, x1, y1);

	float alpha = f_12(xmin, ymin, x1, y1, x2, y2) / f_alpha;
	float beta = f_20(xmin, ymin, x0, y0, x2, y2) / f_beta;
	float gamma = f_01(xmin, ymin, x0, y0, x1, y1) / f_gamma;

	const float f_alpha_off = f_alpha * f_12(X_OFF, Y_OFF, x1, y1, x2, y2);
	const float f_beta_off = f_beta * f_20(X_OFF, Y_OFF, x0, y0, x2, y2);
	const float f_gamma_off = f_gamma * f_01(X_OFF, Y_OFF, x0, y0, x1, y1);

	const float f_alpha_off_backup = f_alpha * f_12(X_OFF_BACKUP, Y_OFF_BACKUP, x1, y1, x2, y2);
	const float f_beta_off_backup = f_beta * f_20(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x2, y2);
	const float f_gamma_off_backup = f_gamma * f_01(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x1, y1);

	const float alpha_x_increment = (y1 - y2) / f_alpha;
	const float beta_x_increment = (y2 - y0) / f_beta;
	const float gamma_x_increment = (y0 - y1) / f_gamma;

	const float alpha_y_increment = (x2 - x1) / f_alpha;
	const float beta_y_increment = (x0 - x2) / f_beta;
	const float gamma_y_increment = (x1 - x0) / f_gamma;

	/* Loop over all pixels in the bounding box */
	for (int y = ymin; y < ymax; y++) {
		/*
			Make a backup of the current barycentric coordinates such that we
			can restore it later.
		*/
		float alpha_backup = alpha;
		float beta_backup = beta;
		float gamma_backup = gamma;

		for (int x = xmin; x < xmax; x++) {
			/* Only draw the pixel if it is inside the triangle */
			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				/*
					And if it is on the edge of the triangle, use the offscreen
					point to determine if the pixel should be drawn. The pixel
					on the same side of the edge as the offscreen point will be
					drawn.
				*/
				if (((alpha > 0) || (f_alpha_off > 0)) &&
					((beta > 0) || (f_beta_off > 0)) &&
					((gamma > 0) || (f_gamma_off > 0))) {

					PutPixel(x, y, r, g, b);
				} else if (
					/*
						In case the shared edge goes through the offscreen point,
						use a backup offscreen point.
					*/
					((alpha > 0) || (f_alpha_off_backup > 0)) &&
					((beta > 0) || (f_beta_off_backup > 0)) &&
					((gamma > 0) || (f_gamma_off_backup > 0))) {

					PutPixel(x, y, r, g, b);
				}
			}

			/* Use an incremental calculation to speedup the algorithm */
			alpha += alpha_x_increment;
			beta += beta_x_increment;
			gamma += gamma_x_increment;
		}

		/* Restore the backupped coordinates such that we can start a new y iteration */
		alpha = alpha_backup;
		beta = beta_backup;
		gamma = gamma_backup;

		alpha += alpha_y_increment;
		beta += beta_y_increment;
		gamma += gamma_y_increment;
	}
}

int getMin(int a, int b, int c) {
	return (a < b ? (a < c ? a : c) : (b < c ? b : c));
}

int getMax(int a, int b, int c) {
	return (a > b ? (a > c ? a : c) : (b > c ? b : c));
}
