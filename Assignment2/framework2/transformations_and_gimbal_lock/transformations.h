/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.h
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 *
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H


void myScalef(GLfloat x, GLfloat y, GLfloat z);
void myTranslatef(GLfloat x, GLfloat y, GLfloat z);
void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
int getSmallest(GLfloat a, GLfloat b, GLfloat c);
void normalize3f(GLfloat *v);
void cross_product3f(GLfloat *x, GLfloat *y, GLfloat *result);

#endif
