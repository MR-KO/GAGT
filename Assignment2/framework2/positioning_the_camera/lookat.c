/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
 * Created by ...... Jurgen Sturm
 *
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ 07-02-2014
 * Comments ........ NULL
 *
 *
 * (always fill in these fields before submitting!!)
 */
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "lookat.h"


/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void normalize3f(GLfloat *v) {
	if (v == NULL) {
		return;
	}

	GLfloat length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] = v[0] / length;
	v[1] = v[1] / length;
	v[2] = v[2] / length;
}

void normalize3d(GLdouble *v) {
	if (v == NULL) {
		return;
	}

	GLdouble length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] = v[0] / length;
	v[1] = v[1] / length;
	v[2] = v[2] / length;
}



void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ) {

	// Change the coordinate system to camera coordinates.
	// For that, we need the three vectors that represent the camera coordinates
	// in world coordinates
	// First the cz vector (pointing from Pcamera to Plookat)
	GLdouble cz[3];

	cz[0] = eyeX - centerX;
	cz[1] = eyeY - centerY;
	cz[2] = eyeZ - centerZ;

	// Normalize cz
	normalize3d(cz);

	// Then the cy vector (points in the direction of the camera cy vector)
	// Get a real orthogonal vector cx first, which is orthogonal to up and cz
	GLdouble cx[3];

	cx[0] = upY * cz[2] - upZ * cz[1];
	cx[1] = upZ * cz[0] - upX * cz[2];
	cx[2] = upX * cz[1] - upY * cz[0];

	// Normalize cx
	normalize3d(cx);

	// Get cy, which is orthogonal to cx and cz
	GLdouble cy[3];

	cy[0] = cz[1] * cx[2] - cz[2] * cx[1];
	cy[1] = cz[2] * cx[0] - cz[0] * cx[2];
	cy[2] = cz[0] * cx[1] - cz[1] * cx[0];

	// Normalize cy
	normalize3d(cy);

	// Get the rotation matrix R and apply it
	GLfloat R[16] = {
		cx[0], cy[0], cz[0], 0,
		cx[1], cy[1], cz[1], 0,
		cx[2], cy[2], cz[2], 0,
		0, 0, 0, 1
	};

	glMultMatrixf(R);
}
