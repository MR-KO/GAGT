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

void determine_octant(double x0, double x1, double y0, double y1) {
	double x = x1 - x0;
	double y = y1 - y0;
	double xy = x - y;

	printf("x0 = %g, y0 = %g, x1 = %g, y1 = %g:  ", x0, y0, x1, y1);
	printf("\tx1 - x0 = %g, y1 - y0 = %g, (x1 - x0) - (y1 - y0) = %g   ",
		x, y, xy);

	if (x >= 0 && y < 0 && xy >= 0) {
		printf("\tFirst octant (0 < m <= 1)");
	}

	if (x >= 0 && y < 0 && xy < 0) {
		printf("\tFifth octant (?)");
	}


	if (x >= 0 && y >= 0 && xy >= 0) {
		printf("\tSeventh octant (0 < m <= -1)");
	}

	if (x >= 0 && y >= 0 && xy < 0) {
		printf("\tSixth octant (-1 < m <= Inf)");
	}



	if (x < 0 && y < 0 && xy >= 0) {
		printf("\tFourth octant (?)");
	}

	if (x < 0 && y < 0 && xy < 0) {
		printf("\tThird octant (?)");
	}


	if (x < 0 && y >= 0 && xy >= 0) {
		printf("Second octant (?)");
	}

	if (x < 0 && y >= 0 && xy < 0) {
		printf("Eight octant (?)");
	}

	printf("\n");
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

	determine_octant(x0, x1, y0, y1);

	int y = y0;
	double d = f(x0 + 1, y0 + 0.5, x0, y0, x1, y1);
    // printf("d = %g\n", d);

	for(int x = x0; x != x1; x += ix) {
		PutPixel(s, x, y, colour);
		// double rv = f(x + 1, y + 0.5, x0, y0, x1, y1);

		// printf("x = %d, y = %d, f = %g, d = %g\n", x, y, rv, d);

		if (iy == 1) {
            if (d < 0) {
                y += iy;
                d = d + ((x1 - x0) + (y0 - y1));
            } else if (d >= 0) {
                d = d + (y0 - y1);
            }
        } else if (iy == -1) {
            if (d >= 0) {
                y += iy;
                d = d + (-(x1 - x0) + (y0 - y1));
            } else if (d < 0) {
                d = d + (y0 - y1);
            }
        }
	}

	return;
}
