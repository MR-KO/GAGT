/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
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

int are_orthogonal(GLfloat *v1, GLfloat *v2, int N) {
    if (N < 1) {
        return -1;
    }

    GLfloat temp = 0;

    for (int i = 0; i < N; i++) {
        temp += v1[i] * v2[i];
    }

    return (temp == 0);
}

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

    GLfloat length = sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    w[0] = w[0] / length;
    w[1] = w[1] / length;
    w[2] = w[2] / length;

    // printf("wx = %g, wy = %g, wz = %g, length = %g\n", w[0], w[1], w[2], length);

    // Compute the value of t, based on w
    int i = getSmallest(fabs(w[0]), fabs(w[1]), fabs(w[2]));

    if (i == 0) {
        t[0] = 1;
        t[1] = w[1];
        t[2] = w[2];
    } else if (i == 1) {
        t[0] = w[0];
        t[0] = 1;
        t[2] = w[2];
    } else if (i == 2) {
        t[0] = w[0];
        t[1] = w[1];
        t[0] = 1;
    }
    // printf("x = %g, y = %g, z = %g\n", t[0], t[1], t[2]);

    // Compute u = t x w  /  || t x w ||
    GLfloat tw[3];
    tw[0] = (t[1] * w[2]) - (t[2] * w[1]);
    tw[1] = (t[2] * w[0]) - (t[0] * w[2]);
    tw[2] = (t[0] * w[1]) - (t[1] * w[0]);

    length = sqrt(tw[0] * tw[0] + tw[1] * tw[1] + tw[2] * tw[2]);

    u[0] = tw[0] / length;
    u[1] = tw[1] / length;
    u[2] = tw[2] / length;

    // printf("x = %g, y = %g, z = %g, length %g\n", tw[0], tw[1], tw[2], length);
    // printf("x = %g, y = %g, z = %g\n", u[0], u[1], u[2]);

    // Normalize u
    length = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);

    u[0] = u[0] / length;
    u[1] = u[1] / length;
    u[2] = u[2] / length;

    // printf("ux = %g, uy = %g, uz = %g, length = %g\n", u[0], u[1], u[2], length);

    // Compute v = w x u
    v[0] = (w[1] * u[2]) - (w[2] * u[1]);
    v[1] = (w[2] * u[0]) - (w[0] * u[2]);
    v[2] = (w[0] * u[1]) - (u[1] * u[0]);

    // Normalize v
    length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    v[0] = v[0] / length;
    v[1] = v[1] / length;
    v[2] = v[2] / length;

    // printf("vx = %g, vy = %g, vz = %g\n", v[0], v[1], v[2]);

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.
    GLfloat ortho = v[0] * u[0] + v[1] * u[1] + v[2] * u[2];

    if (ortho != 0) {
        printf("v and u are not orthogonal! (v.u = %g)\n", ortho);
    }

    ortho = v[0] * w[0] + v[1] * w[1] + v[2] * w[2];

    if (ortho != 0) {
        printf("v and w are not orthogonal! (v.w = %g)\n", ortho);
    }

    ortho = w[0] * u[0] + w[1] * u[1] + w[2] * u[2];

    if (ortho != 0) {
        printf("w and u are not orthogonal! (w.u = %g)\n", ortho);
    }


    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A

    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
    angle = angle * M_PI / 180.0;

    // Specify matrix B

    GLfloat B[16] =
    {
        cos(angle), -1.0 * sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Specify matrix C

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