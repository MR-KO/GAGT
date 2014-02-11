/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.h
 * Description ..... Contains the re-programmed lookAt function
 * Created by ...... Jurgen Sturm
 */


#ifndef IBS_LOOKAT_H
#define IBS_LOOKAT_H


void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ);

void normalize3f(GLfloat *v);
void normalize3d(GLdouble *v);
void cross_product3f(GLfloat *x, GLfloat *y, GLfloat *result);
void cross_product3d(GLdouble *x, GLdouble *y, GLdouble *result);

#endif /* IBS_LOOKAT_H */
