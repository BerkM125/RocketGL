#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include "figureslib.hpp"
#include "rocketcalc.hpp"
#include "metrics.hpp"
GLfloat swidth = 700;
GLfloat sheight = 700;

float angle = -5.0;
float lx = 7.0f, ly = 0.0f, lz = 0.0f;
float x = -30.0f, y = 12.0f, z = 5.0f;

void changeSize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = 1.0 * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 2000);
    glMatrixMode(GL_MODELVIEW);
    swidth = w;
    sheight = h;
}

void gridlines(double step) {
    glBegin(GL_LINES);
    glColor3f(0.9f, 0.9f, 0.9f);
    for (double x = -100; x < 100; x+=step) {
        glVertex3f(x, 0.0f, -100.0f);
        glVertex3f(x, 0.0f, 100.0f);
    }
    for (double y = -100; y < 100; y += step) {
        glVertex3f(-100, 0.0f, y);
        glVertex3f(100, 0.0f, y);
    }
    glEnd();
}

void falconheavy(void) {
    static int r = 0;
    fullrocket falcon9;
    falcon9.rbody.height = 70;
    falcon9.rbody.radius = 1.83f;
    falcon9.rbody.hullthickness = 0.0047;
    falcon9.rbody.accuratebodymass = 1420788;
    falcon9.rbody.totalbodymass = 6000000;
    falcon9.rbody.material = STAINLESSSTEEL;
    falcon9.rbody.rcylinder.xcoord = -35.0f * METER;
    falcon9.rbody.rcylinder.ycoord = -20.0f * METER;
    falcon9.rbody.rcylinder.zcoord = -3.0f * METER;
    falcon9.rbody.rcylinder.vertexnum = 90;
    falcon9.rstaging.payloadmass = 100;
    falcon9.rstaging.propellantmass[0] = 1222800;
    falcon9.rstaging.propellantmass[1] = 107200;
    falcon9.rstaging.structuremass[0] = 51000;
    falcon9.rstaging.structuremass[1] = 4500;
    falcon9.rstaging.specificimpulse = 312;
    falcon9.rbody.calcBodyHeight();
    falcon9.rstaging.calctotalmass(0);
    falcon9.rstaging.calctotalmass(1);
    falcon9.rbody.renderRocketCylinder(GL_TRIANGLE_STRIP);
    falcon9.rstaging.renderStaging(STAGINGTYPEPARALLEL, falcon9.rbody);
    falcon9.rpayload.height = 15.0f;
    falcon9.rpayload.renderRocketTip(GL_POLYGON, falcon9.rbody);
    falcon9.rfins.finheight = 25.0f;
    falcon9.rfins.finwidth = 10.0f;
    falcon9.rfins.finthickness = 0.2f;
    falcon9.rfins.renderFins(true, falcon9);
    if (r == 0) {
        std::cout << "SpaceX Falcon 9 Evaluation:\n";
        rocketdeltaveval(&falcon9.rstaging, 0);
        falcon9.rstaging.specificimpulse = 348;
        rocketdeltaveval(&falcon9.rstaging, 1);
        std::cout << "HEIGHT: " << falcon9.rbody.height << std::endl;
        std::cout << "TOTAL MASS: " << falcon9.rstaging.totalmass[0] << "\n";
        std::cout << "EXHAUST VELOCITY: " << falcon9.rstaging.exhaustvelocity << std::endl;
        std::cout << "SURFACE AREA: " << falcon9.calcSurfaceArea() << std::endl;
        falcon9.rfins.atmosphericDensity = 0.00052;
        falcon9.rfins.averagevelocity = 275;
        falcon9.rfins.dragCoefficient = 0.716;
        falcon9.rfins.liftCoefficient = 0.85;
        std::cout << "LIFT FORCE: " << falcon9.rfins.calcliftforce(1000) << std::endl;
        std::cout << "DRAG FORCE: " << falcon9.rfins.calcdragforce(1000) << std::endl;
        r++;
    }
}

void saturn_V(void) {
    static int r = 0;
    fullrocket saturnV;
    saturnV.rbody.height = 111;
    saturnV.rbody.radius = 3.33;
    saturnV.rbody.hullthickness = 0.0065;
    saturnV.rbody.totalbodymass = 731000 *(1/ saturnV.rbody.hullthickness);
    saturnV.rbody.material = ALUMINUM;
    saturnV.rbody.rcylinder.xcoord = -75.0f * METER;
    saturnV.rbody.rcylinder.ycoord = -20.0f * METER;
    saturnV.rbody.rcylinder.zcoord = -10.0f * METER;
    saturnV.rbody.rcylinder.vertexnum = 90;
    saturnV.rstaging.payloadmass = 0;
    saturnV.rstaging.specificimpulse = 263;
    saturnV.rbody.accuratebodymass = 2900000;
    saturnV.rstaging.totalmass[0] = 2900000;
    saturnV.rstaging.structuremass[0] = 731000;
    saturnV.rstaging.propellantmass[0] = 2169000;
    saturnV.rstaging.totalmass[1] = 600000;
    saturnV.rstaging.structuremass[1] = 156000;
    saturnV.rstaging.propellantmass[1] = 444000;
    saturnV.rstaging.totalmass[2] = 120800;
    saturnV.rstaging.structuremass[2] = 10000;
    saturnV.rstaging.propellantmass[2] = 110800;
    saturnV.rbody.renderRocketCylinder(GL_TRIANGLE_STRIP);
    saturnV.rstaging.renderStaging(STAGINGTYPESERIAL, saturnV.rbody);
    saturnV.rpayload.height = 15.0f;
    saturnV.rpayload.renderRocketTip(GL_POLYGON, saturnV.rbody);
    saturnV.rfins.finheight = 25.0f;
    saturnV.rfins.finwidth = 10.0f;
    saturnV.rfins.finthickness = 1.0f;
    saturnV.rfins.renderFins(false, saturnV);
    if (r == 0) {
        std::cout << "NASA SATURN V Evaluation:\n";
        rocketdeltaveval(&saturnV.rstaging, 0);
        saturnV.rstaging.specificimpulse = 421;
        rocketdeltaveval(&saturnV.rstaging, 1);
        rocketdeltaveval(&saturnV.rstaging, 2);
        std::cout << "HEIGHT: " << saturnV.rbody.height << std::endl;
        std::cout << "TOTAL MASS: " << saturnV.rstaging.totalmass[0] << "\n";
        std::cout << "EXHAUST VELOCITY: " << saturnV.rstaging.exhaustvelocity << std::endl;
        r++;
    }
    cone nozzle(4.0f * METER, 6.0f * METER, saturnV.rbody.rcylinder.xcoord, saturnV.rbody.rcylinder.ycoord, saturnV.rbody.rcylinder.zcoord+saturnV.rbody.height*METER, 360);
    nozzle.drawCone(100, 100, 100, GL_TRIANGLE_STRIP);
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0, 1.0f, 0.0);
    gridlines(2);
    falconheavy();
    saturn_V();
    glScalef(0.0525, 0.0525, 0.0525);
    glRotatef(-90, 0, 0.1, 0);
    RenderString(0, 400, 400, GLUT_STROKE_MONO_ROMAN, "SPACEX FALCON 9");
    RenderString(-1500, 0, 550, GLUT_STROKE_MONO_ROMAN, "NASA'S SATURN V");
    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    float fraction = 0.5f; //Almost like a sensitivity for the keys
    if (key == 'w') { // W
        x += lx * fraction;
        z += lz * fraction;
    }
    if (key == 's') { // S
        x -= lx * fraction;
        z -= lz * fraction;
    }
    if (key == 'd') { // D
        angle += 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
    }
    if (key == 'a') { // A
        angle -= 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
    }
    if (key == 27)
        exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {
    float fraction = 0.2f; //Sensitivity factor for the keys
    switch (key) {
    case GLUT_KEY_UP:
        y += 0.5f;
        break;
    case GLUT_KEY_DOWN:
        y -= 0.5f;
        break;
    }
}

void mousemove(int x, int y) {
    static GLfloat px = 0;
    static GLfloat py = 0;
    GLfloat x1 = x / (GLfloat)(swidth / 2) - 1.0;
    GLfloat y1 = y / (GLfloat)(sheight / 2) - 1.0;
    if (px == 0 && py == 0) {
        px = x1;
        py = y1;
    }
    else {
        angle += (x1 - px) * 3; //This 3.0 is a sensitivity factor but specific for X and Z plane rotation
        lx = sin(angle);
        lz = -cos(angle);
        ly += (py - y1) * 0.8; //This 0.8 is a sensitivity factor but specific for Y rotation
        px = x1;
        py = y1;
    }
    return;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Player");
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMotionFunc(mousemove);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 1;
}