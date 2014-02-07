/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Kevin Ouwehand & Timo Dobber
 * Student email ...  & timo.dobber@gmail.com
 * Collegekaart .... & 10386726
 * Date ............ 7/2/2014
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include "SDL.h"
#include "init.h"
#include "mla.h"

double f(double x, double y, double x0, double y0, double x1, double y1) {
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

int determine_octant(double x0, double x1, double y0, double y1) {
    double x = x1 - x0;
    double y = y1 - y0;
    double xy = x - y;

    if (x > 0) {
        if (y >= 0) {
            xy = x - y;
        } else if (y < 0) {
            xy = x + y;
        }
    } else if (x < 0) {
        if (y >= 0) {
            xy = x + y;
        } else if (y < 0) {
            xy = x - y;
        }
    } else if (x == 0) {
        xy = x + y;
    }

    if (x > 0 && y < 0 && xy >= 0) {
        return 1;
    }

    if (x >= 0 && y <= 0 && xy < 0) {
        return 2;
    }

    if (x < 0 && y < 0 && xy >= 0) {
        return 3;
    }

    if (x < 0 && y <= 0 && xy < 0) {
        return 4;
    }

    if (x < 0 && y > 0 && xy <= 0) {
        return 5;
    }

    if (x <= 0 && y >= 0 && xy > 0) {
        return 6;
    }

    if (x > 0 && y >= 0 && xy <= 0) {
        return 7;
    }

    if (x > 0 && y >= 0 && xy > 0) {
        return 8;
    }

    return -1;
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

    if (octant == -1) {
        return;
    }

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

    if (octant == 2 || octant == 3 || octant == 6 || octant == 7) {
        *to_increment = x0;
        double d = f(x0 + 1, y0 + 0.5, x0, y0, x1, y1);

        for (int y = y0; y != y1; y += iy) {
            PutPixel(s, *to_increment, y, colour);

            if (ix == 1) {
                if (octant == 2 || octant == 6) {
                    d = checkConditions(d, x0, x1, y1, y0, x0, x1, -1, ix, to_increment);
                } else {
                    d = checkConditions(d, x0, x1, y1, y0, x0, x1, 1, ix, to_increment);
                }
            }
        }
    } else {
        *to_increment = y0;
        double d = f(x0 + 1, y0 + 0.5, x0, y0, x1, y1);

        for (int x = x0; x != x1; x += ix) {
            PutPixel(s, x, *to_increment, colour);
            d = checkConditions(d, x0, x1, x1, x0, y0, y1, iy, iy, to_increment);
        }
    }

    return;
}

double checkConditions(double d, int x0, int x1, int left_1, int left_2, int right_1, int right_2,
    int sign, int increment, int* to_increment) {

    if ((increment == 1 && d < 0) || (increment == -1 && d >= 0)) {
        if (x1 - x0 == 0) {
            d = d + (right_1 - right_2);
        }
        else {
            (*to_increment) += increment;
            d = d + (sign * (left_1 - left_2) + (right_1 - right_2));
        }
    } else if ((increment == 1 && d >= 0) || (increment == -1 && d < 0)) {
        d = d + (right_1 - right_2);
    }

    return d;
}