/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.h
 * Description ..... Header file for mla.c
 * Created by ...... Jurgen Sturm
 *
 */

#ifndef JS_MLA_H
#define JS_MLA_H

void determine_octant(double x0, double y0, double x1, double y1);
void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour);

#endif /* JS_MLA_H */
