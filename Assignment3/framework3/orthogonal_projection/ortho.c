/* Computer Graphics, Assignment 5, Orthogonal projection
 *
 * Filename ........ ortho.c
 * Description ..... Contains the re-programmed orthogonal projection matrix
 * Date ............ 01.09.2006
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

 
#define sqr(x) ((x)*(x))

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
             GLdouble near, GLdouble far) {

	GLdouble T[16] =
		{1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 -((left + right) / 2), -((bottom + top) / 2), -((near + far) / 2), 1};

	GLdouble S[16] = 
		{(2 / (right - left)), 0, 0, 0,
		 0, (2 / (top - bottom)), 0, 0,
		 0, 0, (2 / (near - far)), 0,
		 0, 0, 0, 1};

	glMultMatrixd(S);
	glMultMatrixd(T);
}
