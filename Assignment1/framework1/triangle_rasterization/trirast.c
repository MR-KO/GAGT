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

void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b) {

	int xmin, xmax, ymin, ymax;
	float alpha, beta, gamma;

	xmin = getMin(x0, x1, x2);
	ymin = getMin(y0, y1, y2);
	xmax = getMax(x0, x1, x2);
	ymax = getMax(y0, y1, y2);

	for (int y = ymin; y <= ymax; y++) {
		for (int x = xmin; x <= xmax; x++) {
			beta = f_20(x, y, x0, y0, x2, y2) / f_20(x1, y1, x0, y0, x2, y2);
			gamma = f_01(x, y, x0, y0, x1, y1) / f_01(x2, y2, x0, y0, x1, y1);
			alpha = 1 - beta - gamma;
			// printf("alpha: %g, beta: %g, gamma: %g\n", alpha, beta, gamma);
			if (alpha > 0 && beta > 0 && gamma > 0) {
				PutPixel(x, y, r, g, b);
			}
		}
	}

}

void draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b) {


}

int getMin(int a, int b, int c) {
	if (a < b) {
		if (a < c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b < c) {
			return b;
		}
		else {
			return c;
		}
	}
}

int getMax(int a, int b, int c) {
	if (a > b) {
		if (a > c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b > c) {
			return b;
		}
		else {
			return c;
		}
	}
}
