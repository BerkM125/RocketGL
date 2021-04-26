#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "figureslib.hpp"

extern GLfloat swidth;
extern GLfloat sheight;

void RenderString(float x, float y, float z, void* font, const char* string) {
    char* c;
    int n = 0;
    glColor3f(255, 255, 255);
    glTranslatef(x, y, z);
    while (string[n] != '\0') {
        glutStrokeCharacter(font, string[n]);
        n++;
    }
}

cylinder::cylinder(GLfloat rad, GLfloat hgt, GLfloat x, GLfloat y, GLfloat z, GLfloat vertices) {
    radius = rad;
    height = hgt;
    xcoord = x;
    ycoord = y;
    zcoord = z;
    vertexnum = vertices;
}

void cylinder::drawCylinder(GLubyte R, GLubyte G, GLubyte B, GLenum primtype) {
    GLfloat x = xcoord;
    GLfloat y = ycoord;
    GLfloat z = zcoord;
    GLfloat angle = 0.0;
    GLfloat step = (6.33 / vertexnum);
    GLfloat dblPI = 2 * PI;
    GLfloat colortint = 40;
    glColor3ub(R - colortint, G - colortint, B - colortint);
    glBegin(primtype);
    angle = 0.0;
    while (angle < dblPI) {
        x = (radius * cos(angle)) + xcoord;
        y = (radius * sin(angle)) - ycoord;
        glVertex3f(x, y, height + z);
        glVertex3f(x, y, z);
        angle = angle + step;
    }
    glVertex3f(radius + xcoord, -ycoord, height + z);
    glVertex3f(radius + xcoord, -ycoord, z);
    glEnd();
    glColor3ub(R, G, B);
    glBegin(GL_POLYGON);
    angle = 0.0;
    while (angle < dblPI) {
        x = (radius * cos(angle)) + xcoord;
        y = (radius * sin(angle)) - ycoord;
        glVertex3f(x, y, z);
        angle = angle + step;
    }
    glVertex3f(radius + xcoord, -ycoord, z);
    glEnd();
}

cone::cone(GLfloat rad, GLfloat hgt, GLfloat x, GLfloat y, GLfloat z, GLfloat vertices) {
    radius = rad;
    height = hgt;
    xcoord = x;
    ycoord = y;
    zcoord = z;
    vertexnum = vertices;
}

void cone::drawCone(GLubyte R, GLubyte G, GLubyte B, GLenum primtype) {
    GLfloat x = xcoord;
    GLfloat y = ycoord;
    GLfloat z = zcoord;
    GLfloat angle = 0.0;
    GLfloat step = (6.33 / vertexnum);
    GLfloat dblPI = 2 * PI;
    GLfloat colortint = 40;
    glColor3ub(R - colortint, G - colortint, B - colortint);
    glBegin(primtype);
    angle = 0.0;
    while (angle < dblPI) {
        x = (radius * cos(angle)) + xcoord;
        y = (radius * sin(angle)) - ycoord;
        glVertex3f(x, y, height + z);
        glVertex3f(xcoord, -ycoord, z);
        angle = angle + step;
    }
    glEnd();
    glColor3ub(R, G, B);
    glBegin(GL_POLYGON);
    angle = 0.0;
    while (angle < dblPI) {
        x = (radius * cos(angle)) + xcoord;
        y = (radius * sin(angle)) - ycoord;
        glVertex3f(x, y, height + z);
        angle = angle + step;
    }
    glVertex3f(radius + xcoord, -ycoord, height + z);
    glEnd();
}

cube::cube(GLfloat x, GLfloat y, GLfloat z, GLfloat cw) {
    xcoord = x;
    ycoord = y;
    zcoord = z;
    cubewidth = cw;
}

void cube::drawCube(GLubyte R, GLubyte G, GLubyte B, GLenum primtype) {
    GLfloat x = xcoord;
    GLfloat y = ycoord;
    GLfloat z = zcoord;
    GLfloat colortint = 40;
    glColor3ub(R - colortint, G - colortint, B - colortint);
    glBegin(primtype);
    glVertex3f(x, y, z);
    glVertex3f(x, y, cubewidth + z);
    glVertex3f(x + cubewidth, y, z);
    glVertex3f(x + cubewidth, y, cubewidth + z);
    glVertex3f(x + cubewidth, y + cubewidth, z);
    glVertex3f(x + cubewidth, y + cubewidth, cubewidth + z);
    glVertex3f(x, y + cubewidth, z);
    glVertex3f(x, y + cubewidth, cubewidth + z);
    glVertex3f(x, y, z);
    glVertex3f(x, y + cubewidth, z);
    glVertex3f(x, y, cubewidth + z);
    glVertex3f(x, y + cubewidth, cubewidth + z);
    glVertex3f(x, y, z);
    glVertex3f(x + cubewidth, y, z);
    glVertex3f(x, y + cubewidth, z);
    glVertex3f(x + cubewidth, y + cubewidth, z);
    glEnd();
}

void sphere::rendersphere(GLubyte R, GLubyte G, GLubyte B, GLenum primtype) {
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat z = 0.0;
    GLfloat xy = 0.0;
    GLfloat lengthInv = 1.0 / (GLfloat)(((swidth + sheight) / 2) / 2) - 1.0;
    GLfloat dblpi = 2 * PI;
    GLfloat sectorstep = dblpi / sectors;
    GLfloat stackstep = PI / stacks;
    GLfloat stackangle = 0.0;
    GLfloat sectorangle = 0.0;
    GLfloat colortint = 40;
    glColor3ub(R - colortint, G - colortint, B - colortint);
    glBegin(primtype);
    for (int i = 0; i <= stacks; ++i) {
        stackangle = PI / 2 - i * stackstep;
        xy = radius * cosf(stackangle);
        z = radius * sinf(stackangle) * lengthInv;
        for (int i2 = 0; i2 <= sectors; ++i2) {
            sectorangle = i2 * sectorstep;
            x = xy * cosf(sectorangle) * lengthInv;
            y = xy * sinf(sectorangle) * lengthInv;
            glVertex3f(x + xcoord, y + ycoord, z + zcoord);
        }
    }
    glEnd();
}

sphere::sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLint sec, GLint stk) {
    xcoord = x;
    ycoord = y;
    zcoord = z;
    radius = r;
    sectors = sec;
    stacks = stk;
}
