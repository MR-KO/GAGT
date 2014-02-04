/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
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

#include "SDL.h"
#include "init.h"

inline double f_01(int x, int y, double x0, double y0, double x1, double y1) {
	return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

inline double f_12(int x, int y, double x1, double y1, double x2, double y2) {
	return (y1 - y1) * x + (x2 - x1) * y + x1 * y2 - x2 * y1;
}

inline double f_20(int x, int y, double x0, double y0, double x2, double y2) {
	return (y2 - y0) * x + (x0 - x2) * y + x2 * y0 - x0 * y2;
}



double f(double x, double y, double x0, double y0, double x1, double y1) {
	return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

/*
 * Midpoint Line Algorithm
 *
 * As you probably will have figured out, this is the part where you prove
 * your programming skills. The code in the mla function should draw a direct
 * line between (x0,y0) and (x1,y1) in the specified color.
 *
 * Until now, the example code below draws only a horizontal line between
 * (x0,y0) and (x1,y0) and a vertical line between (x1,y1).
 *
 * And here the challenge begins..
 *
 * Good luck!
 *
 * (0, 0) is in the top-left corner
 *
 */
void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
	int ix, iy;

	// PutPixel(s, x0, y0, colour);
	// PutPixel(s, x1, y1, colour);

	if(x1 > x0) {
		ix = 1;
	} else {
		ix = -1;
	}

	if(y1 > y0) {
		iy = 1;
	} else {
		iy = -1;
	}

	int y = y0;

	for(int x = x0; x != x1; x += ix) {
		PutPixel(s, x, y, colour);
		double rv = f(x + 1, y + 0.5, x0, y0, x1, y1);

		printf("x = %d, y = %d, f = %g\n", x, y, rv);

		if (iy == -1 && rv > 0.0) {
			printf("Incremented y!\n");
			y += iy;
		}

		if (iy == 1 && rv < 0.0) {
			printf("Incremented y!\n");
			y += iy;
		}
	}

	return;
}
