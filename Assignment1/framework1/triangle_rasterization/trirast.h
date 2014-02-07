#ifndef TRIRAST_H
#define TRIRAST_H

#include "types.h"

void    draw_triangle(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);
void    draw_triangle_optimized(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);

int getMin(int a, int b, int c);
int getMax(int a, int b, int c);

#define X_OFF -1
#define Y_OFF -1

#define X_OFF_BACKUP -1
#define Y_OFF_BACKUP -2

#endif

