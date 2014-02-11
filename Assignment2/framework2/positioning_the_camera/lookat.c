/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
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
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "lookat.h"


/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ) {

	// Translate by the vector -VRP
	//glTranslatef(-eyeX, -eyeY, -eyeZ);

	// Change the coordinate system to camera coordinates.
	// For that, we need the three vectors that represent the camera coordinates
	// in world coordinates
	// First the cz vector (pointing from Pcamera to Plookat)
	GLdouble cz[3];

	cz[0] = eyeX - centerX;
	cz[1] = eyeY - centerY;
	cz[2] = eyeZ - centerZ;

	// Normalize cz
	GLdouble length = sqrt(cz[0] * cz[0] + cz[1] * cz[1] + cz[2] * cz[2]);

	cz[0] = cz[0] / length;
	cz[1] = cz[1] / length;
	cz[2] = cz[2] / length;

	// Then the cy vector (points in the direction of the camera cy vector)
	// Get a real orthogonal vector cx first, which is orthogonal to up and cz
	GLdouble cx[3];

	cx[0] = upY * cz[2] - upZ * cz[1];
	cx[1] = upZ * cz[0] - upX * cz[2];
	cx[2] = upX * cz[1] - upY * cz[0];

	// Normalize cx
	length = sqrt(cx[0] * cx[0] + cx[1] * cx[1] + cx[2] * cx[2]);

	cx[0] = cx[0] / length;
	cx[1] = cx[1] / length;
	cx[2] = cx[2] / length;

	// Get cy, which is orthogonal to cx and cz
	GLdouble cy[3];

	cy[0] = cz[1] * cx[2] - cz[2] * cx[1];
	cy[1] = cz[2] * cx[0] - cz[0] * cx[2];
	cy[2] = cz[0] * cx[1] - cz[1] * cx[0];

	// Normalize cy
	length = sqrt(cy[0] * cy[0] + cy[1] * cy[1] + cy[2] * cy[2]);

	cy[0] = cy[0] / length;
	cy[1] = cy[1] / length;
	cy[2] = cy[2] / length;

	// Get the rotation matrix R
	GLfloat R[16] = {
		cx[0], cy[0], cz[0], 0,
		cx[1], cy[1], cz[1], 0,
		cx[2], cy[2], cz[2], 0,
		0, 0, 0, 1
	};

	// Get the transposed rotation matrix RT
	GLfloat RT[16] = {
		cx[0], cx[1], cx[2], 0,
		cy[0], cy[1], cy[2], 0,
		-cz[0], -cz[1], -cz[2], 0,
		0, 0, 0, 1
	};

	// Get the translate matrix E
	GLfloat E[16] = {
		1, 0, 0, eyeX,
		0, 1, 0, eyeY,
		0, 0, 1, eyeZ,
		0, 0, 0, 1
	};

	glMultMatrixf(R);
	// glMultMatrixf(R);
}
