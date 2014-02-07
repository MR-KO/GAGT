/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.h
 * Description ..... Header file for mla.c
 * Created by ...... Jurgen Sturm
 *
 */

#ifndef JS_MLA_H
#define JS_MLA_H

int determine_octant(double x0, double y0, double x1, double y1);
void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour);
double check_conditions(double d, int x0, int x1, int left_1, int left_2, int right_1, int right_2,
	int sign, int increment_value, int* to_increment);

#define OCTANT_1 1
#define OCTANT_2 2
#define OCTANT_3 3
#define OCTANT_4 4
#define OCTANT_5 5
#define OCTANT_6 6
#define OCTANT_7 7
#define OCTANT_8 8

#define OCTANT_NONE -1

#endif /* JS_MLA_H */
