/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "trirast.h"

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */

inline double f_01(int x, int y, double x0, double y0, double x1, double y1) {
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

inline double f_12(int x, int y, double x1, double y1, double x2, double y2) {
    return (y1 - y1) * x + (x2 - x1) * y + x1 * y2 - x2 * y1;
}

inline double f_20(int x, int y, double x0, double y0, double x2, double y2) {
    return (y2 - y0) * x + (x0 - x2) * y + x2 * y0 - x0 * y2;
}

void draw_triangle(double x0, double y0, double x1, double y1, double x2, double y2,
    byte r, byte g, byte b) {

	double alpha = 0, beta = 0, gamma = 0;

	int xmin = getMin(x0, x1, x2);
	int xmax = getMax(x0, x1, x2);

	int ymin = getMin(y0, y1, y2);
	int ymax = getMax(y0, y1, y2);

	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			beta = f_20(x, y, x0, y0, x2, y2) / f_20(x1, y1, x0, y0, x2, y2);
			gamma = f_01(x, y, x0, y0, x1, y1) / f_01(x2, y2, x0, y0, x1, y1);
			alpha = 1 - beta - gamma;

			// printf("alpha: %g, beta: %g, gamma: %g", alpha, beta, gamma);

			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				if (((alpha > 0) || (f_12(x0, y0, x1, y1, x2, y2) * f_12(X_OFF, Y_OFF, x1, y1, x2, y2) > 0)) &&
					((beta > 0) || (f_20(x1, y1, x0, y0, x2, y2) * f_20(X_OFF, Y_OFF, x0, y0, x2, y2) > 0)) &&
					((gamma > 0) || (f_01(x2, y2, x0, y0, x1, y1) * f_01(X_OFF, Y_OFF, x0, y0, x1, y1) > 0))) {

					PutPixel(x, y, r, g, b);
				} else if (
					((alpha > 0) || (f_12(x0, y0, x1, y1, x2, y2) * f_12(X_OFF_BACKUP, Y_OFF_BACKUP, x1, y1, x2, y2) > 0)) &&
					((beta > 0) || (f_20(x1, y1, x0, y0, x2, y2) * f_20(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x2, y2) > 0)) &&
					((gamma > 0) || (f_01(x2, y2, x0, y0, x1, y1) * f_01(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x1, y1) > 0))) {

					PutPixel(x, y, r, g, b);
				}
			}
		}
	}
}

void draw_triangle_optimized(double x0, double y0, double x1, double y1, double x2, double y2,
    byte r, byte g, byte b) {

	int xmin = (int) floor(getMin(x0, x1, x2));
	int xmax = (int) ceil(getMax(x0, x1, x2));

	int ymin = (int) floor(getMin(y0, y1, y2));
	int ymax = (int) ceil(getMax(y0, y1, y2));

	double f_alfa = f_12(x0, y0, x1, y1, x2, y2);
	double f_beta = f_20(x1, y1, x0, y0, x2, y2);
	double f_gamma = f_01(x2, y2, x0, y0, x1, y1);

	double f_alfa_off = f_alfa * f_12(X_OFF, Y_OFF, x1, y1, x2, y2);
	double f_beta_off = f_beta * f_20(X_OFF, Y_OFF, x0, y0, x2, y2);
	double f_gamma_off = f_gamma * f_01(X_OFF, Y_OFF, x0, y0, x1, y1);

	double f_alfa_off_backup = f_alfa * f_12(X_OFF_BACKUP, Y_OFF_BACKUP, x1, y1, x2, y2);
	double f_beta_off_backup = f_beta * f_20(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x2, y2);
	double f_gamma_off_backup = f_gamma * f_01(X_OFF_BACKUP, Y_OFF_BACKUP, x0, y0, x1, y1);

	double d_alfa = f_12(xmin, ymin, x1, y1, x2, y2);
	double d_beta = f_20(xmin, ymin, x0, y0, x2, y2);
	double d_gamma = f_01(xmin, ymin, x0, y0, x1, y1);

	double beta = f_20(x, y, x0, y0, x2, y2);
	double gamma = f_01(x, y, x0, y0, x1, y1);
	double alpha = 1 - beta - gamma;

	for (int y = ymin; y <= ymax; y++) {
		for (int x = xmin; x <= xmax; x++) {
			beta = f_20(x, y, x0, y0, x2, y2) / f_beta;
			gamma = f_01(x, y, x0, y0, x1, y1) / f_gamma;
			alpha = 1 - beta - gamma;

			// printf("alpha: %g, beta: %g, gamma: %g", alpha, beta, gamma);

			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				if (((alpha > 0) || (f_alfa_off > 0)) &&
					((beta > 0) || (f_beta_off  > 0)) &&
					((gamma > 0) || (f_gamma_off > 0))) {

					PutPixel(x, y, r, g, b);
				} else if (
					((alpha > 0) || (f_alfa_off_backup > 0)) &&
					((beta > 0) || (f_beta_off_backup > 0)) &&
					((gamma > 0) || (f_gamma_off_backup > 0))) {

					PutPixel(x, y, r, g, b);
				}
			}
		}
	}
}

int getMin(int a, int b, int c) {
	return (a < b ? (a < c ? a : c) : (b < c ? b : c));
	// if (a < b) {
	// 	if (a < c) {
	// 		return a;
	// 	}
	// 	else {
	// 		return c;
	// 	}
	// }
	// else {
	// 	if (b < c) {
	// 		return b;
	// 	}
	// 	else {
	// 		return c;
	// 	}
	// }
}

int getMax(int a, int b, int c) {
	return (a > b ? (a > c ? a : c) : (b > c ? b : c));
	// if (a > b) {
	// 	if (a > c) {
	// 		return a;
	// 	}
	// 	else {
	// 		return c;
	// 	}
	// }
	// else {
	// 	if (b > c) {
	// 		return b;
	// 	}
	// 	else {
	// 		return c;
	// 	}
	// }
}
