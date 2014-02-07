#ifndef TRIRAST_H
#define TRIRAST_H

#include "types.h"

void    draw_triangle(double x0, double y0, double x1, double y1,
            double x2, double y2, byte r, byte g, byte b);
void    draw_triangle_optimized(double x0, double y0, double x1, double y1,
            double x2, double y2, byte r, byte g, byte b);

int getMin(int a, int b, int c);
int getMax(int a, int b, int c);

#define X_OFF -1
#define Y_OFF -1

#define X_OFF_BACKUP -1
#define Y_OFF_BACKUP -2

#endif

