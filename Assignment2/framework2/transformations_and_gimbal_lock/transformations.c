/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ 11-02-2014
 * Comments ........ NULL
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "transformations.h"

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myScalef(GLfloat x, GLfloat y, GLfloat z) {

    GLfloat M[16] =
    {
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixf(M);
}


void myTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    };

    glMultMatrixf(M);
}

void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];

    //
    // 1. Create the orthonormal basis
    //

    // Store the incoming rotation axis in w and normalize w
    w[0] = x;
    w[1] = y;
    w[2] = z;

    normalize3f(w);

    // Compute the value of t, based on w, and set the smallest magnituted value to 1
    int i = getSmallest(fabs(w[0]), fabs(w[1]), fabs(w[2]));

    t[0] = w[0];
    t[1] = w[1];
    t[2] = w[2];
    t[i] = 1;

    // Compute u = t x w  /  || t x w ||
    GLfloat tw[3];
    cross_product3f(t, w, tw);

    GLfloat length = sqrt(tw[0] * tw[0] + tw[1] * tw[1] + tw[2] * tw[2]);

    u[0] = tw[0] / length;
    u[1] = tw[1] / length;
    u[2] = tw[2] / length;

    // Normalize u
    normalize3f(u);

    // Compute v = w x u
    cross_product3f(w, u, v);

    // Normalize v
    normalize3f(v);

    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A consisting of u, v, and w

    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
    angle = -1.0 * angle * M_PI / 180.0;

    // Specify matrix B that rotates over angle over the z-axis

    GLfloat B[16] =
    {
        cos(angle), -1.0 * sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Specify matrix C consisting of u, v, and w

    GLfloat C[16] =
    {
        u[0], v[0], w[0], 0.0,
        u[1], v[1], w[1], 0.0,
        u[2], v[2], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    //
    // 3. Apply the matrices to get the combined rotation
    //

    glMultMatrixf(A);
    glMultMatrixf(B);
    glMultMatrixf(C);
}


int getSmallest(GLfloat a, GLfloat b, GLfloat c) {
    return (a < b ? (a < c ? 0 : 2) : (b < c ? 1 : 2));
}

void normalize3f(GLfloat *v) {
    GLfloat length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    v[0] = v[0] / length;
    v[1] = v[1] / length;
    v[2] = v[2] / length;
}

void cross_product3f(GLfloat *x, GLfloat *y, GLfloat *result) {
    result[0] = (x[1] * y[2]) - (x[2] * y[1]);
    result[1] = (x[2] * y[0]) - (x[0] * y[2]);
    result[2] = (x[0] * y[1]) - (x[1] * y[0]);
}