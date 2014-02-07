/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Kevin Ouwehand & Timo Dobber
 * Student email ... c0nd3mn3d@hotmail.com & timo.dobber@gmail.com
 * Collegekaart .... 10420908 & 10386726
 * Date ............ 07/02/2014
 * Comments ........ NULL
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include "SDL.h"
#include "init.h"
#include "mla.h"

/* Define the function used to calculate d. */
double f(double x, double y, double x0, double y0, double x1, double y1) {
	return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

/*
 * Determines in which octant the line will fall, first octant is 0-45 degrees,
 * second octant is 45-90 degrees and so on.
 */
int determine_octant(double x0, double x1, double y0, double y1) {
	double x = x1 - x0;
	double y = y1 - y0;
	double xy = x - y;

	/* Calculate the signs and octant type */
	if (x > 0) {
		if (y >= 0) {
			xy = x - y;
		} else {
			xy = x + y;
		}
	} else if (x < 0) {
		if (y >= 0) {
			xy = x + y;
		} else {
			xy = x - y;
		}
	} else if (x == 0) {
		xy = x + y;
	}

	/* Determine the octant */
	if (x > 0 && y < 0 && xy >= 0) {
		return OCTANT_1;
	}

	if (x >= 0 && y <= 0 && xy < 0) {
		return OCTANT_2;
	}

	if (x < 0 && y < 0 && xy >= 0) {
		return OCTANT_3;
	}

	if (x < 0 && y <= 0 && xy < 0) {
		return OCTANT_4;
	}

	if (x < 0 && y > 0 && xy <= 0) {
		return OCTANT_5;
	}

	if (x <= 0 && y >= 0 && xy > 0) {
		return OCTANT_6;
	}

	if (x > 0 && y >= 0 && xy <= 0) {
		return OCTANT_7;
	}

	if (x > 0 && y >= 0 && xy > 0) {
		return OCTANT_8;
	}

	return OCTANT_NONE;
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
	int ix = 1, iy = 1, temp = -1;
	int octant = determine_octant(x0, x1, y0, y1);
	int* to_increment = malloc(sizeof(int));

	if (to_increment == NULL) {
		return;
	}

	if (octant == OCTANT_NONE) {
		return;
	}

	/* Swap x1 and x0 when x1 is smaller than x0. */
	if (x1 > x0) {
		ix = 1;
	} else {
		temp = x1;
		x1 = x0;
		x0 = temp;

		temp = y1;
		y1 = y0;
		y0 = temp;
	}

	if (y1 > y0) {
		iy = 1;
	} else {
		iy = -1;
	}

	/*
		Checks whether the line is steep, because then the x and y need to
		be swapped.
	*/
	if (octant == OCTANT_2 || octant == OCTANT_3 || octant == OCTANT_6 ||
		octant == OCTANT_7) {

		(*to_increment) = x0;
		double d = f(x0 + 1, y0 + 0.5, x0, y0, x1, y1);

		for (int y = y0; y != y1; y += iy) {
			PutPixel(s, *to_increment, y, colour);

			/*
			 *  Octant 2 and 6 need to be seperated since they both have an
			 *  exact vertical line, in opposite directions.
			 */
			if (octant == OCTANT_2 || octant == OCTANT_6) {
				int sign = -1;

				d = check_conditions(d, x0, x1, y1, y0, x0, x1, sign, ix,
					to_increment);
			} else {
				int sign = 1;
				d = check_conditions(d, x0, x1, y1, y0, x0, x1, sign, ix,
					to_increment);
			}
		}
	} else {
		/* Here we can use the "regular" method of looping over x instead of y */
		(*to_increment) = y0;
		double d = f(x0 + 1, y0 + 0.5, x0, y0, x1, y1);

		for (int x = x0; x != x1; x += ix) {
			PutPixel(s, x, *to_increment, colour);

			/* Here the sign equals iy */
			d = check_conditions(d, x0, x1, x1, x0, y0, y1, iy, iy, to_increment);
		}
	}

	return;
}

/*
 * Checks all the conditions and calculates the new d incremental.
 */
double check_conditions(double d, int x0, int x1, int left_1, int left_2,
	int right_1, int right_2, int sign, int increment_value, int* to_increment) {

	if ((increment_value == 1 && d < 0) || (increment_value == -1 && d >= 0)) {
		if (x1 - x0 == 0) {
			d = d + (right_1 - right_2);
		} else {
			(*to_increment) += increment_value;
			d = d + (sign * (left_1 - left_2) + (right_1 - right_2));
		}
	} else if ((increment_value == 1 && d >= 0) || (increment_value == -1 && d < 0)) {
		d = d + (right_1 - right_2);
	}

	return d;
}
