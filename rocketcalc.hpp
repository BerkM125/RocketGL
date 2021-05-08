#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include "figureslib.hpp"
#ifndef ROCKETPHYSICS_ROCKETCALC_H
#define ROCKETPHYSICS_ROCKETCALC_H

#endif //ROCKETPHYSICS_ROCKETCALC_H

/*NOTE: All units used for the classes must abide with the metric system and standard scientific units of force and pressure*/
#pragma once
#define AIRPRESSURE_100M 14.53
#define GRAVITY 9.81
#define AVERAGEDENSITYSLOPE -0.00006
#define SPECIFICIMPULSEDERIVATION 0
#define VELOCITYDERIVATION 1
//Types of staging for rendering stages. Parallel staging will render boosters in parallel, serial will divide rocket up into different stages.
#define STAGINGTYPEPARALLEL 1
#define STAGINGTYPESERIAL 2
class fullrocket;
//PARENT ROCKET CLASS: houses main, bigger picture, whole-rocket variables and functions, as well as sub-classes of different rocket parts.
class rocket {
public:
    cylinder rcylinder;
    int material;
    double height;
    double radius;
    double totalbodymass; //totalbodymass is only for hull rendering
    double accuratebodymass; //accuratebodymass is for mass calculations
    double hullthickness;
    double massflowrate;
    double dMpropellant;
    double impulse;
    double specificimpulse;
    double thrustforce;
    double exhaustvelocity;
    double initialmass;
    double finalmass;
    double massratio;
    double averagevelocity;
    double calcmassratio();
    double calcthrustforce(int derivation);
    double calcchangepropellant(int derivation);
    double calcimpulse (int derivation);
    void passMeasurements(void);
    double calcRocketMass(void);
    void renderRocketCylinder(GLenum primtype);
    void calcBodyHeight(void);
    rocket(double mtotal = 0, double mdot = 0, double dMprop = 0, double i = 0, double isp = 0, double fthrust = 0, double c = 0, double minitial = 0, double mfinal = 0, double mr = 0);

};

//ROCKET TIP SUB-CLASS: houses variables and functions involving rocket tip
class tip : public rocket {
public:
    cylinder tipbody;
    double mass;
    double height;
    int material;
    void renderRocketTip(GLenum primtype, rocket body);
    void calcTipMass(void);
};

//ROCKET NOZZLE SUB-CLASS: houses variables and functions involving the rocket's nozzle.
class nozzle : public rocket {
public:
    cylinder combustionchamber;
    cone nozzletip;
    int material;
    double mass;
    double exhaustarea;
    double exhaustpressure;
    double outsidepressure;
    double pressurechange;
    double nozzleforce;
    void setnozzleforce(double fnozzle);
    void renderNozzle(void);
    void calcNozzleMass(void);
    double calcnozzleforce();
    double calcthrustforce();
};
//ROCKET ENGINE SUB-CLASS: houses variables and functions involving rocket and its engine. THIS CLASS SHOULD BE USED WHEN CREATING A ROCKET
class engine : public rocket {
public:
    int fuel;
    int oxidizer;
    int hullmaterial;
    double chamberpressure;
};
//ROCKET STAGE SUB-CLASS: houses variables and functions involving a multistaged rocket's stages
class stage : public rocket {
public:
    int stagenum;
    double structuremass[8]{};
    double propellantmass[8]{};
    double totalmass[8]{};
    double massratio[8]{};
    double deltav[8]{};
    double totaldeltav;
    double payloadmass;
    double boosterheight[8];
    double boosterradius[8];
    double calcdeltav (int stage);
    double calcmassratio (int stage);
    double calcinitialmass (int stage);
    double calcfinalmass (int stage);
    double calctotalmass (int stage);
    void renderStaging(int stagingtype, rocket mrocket);
    stage();
};
//ROCKET LIFT COMPONENT SUB-CLASS: houses variables and functions involving aerodynamic forces on the rocket
class aerodynamics : public rocket {
public:
    triprism twinfins[2];
    double finheight;
    double finwidth;
    double finthickness;
    double liftcomponentArea;
    double dragCoefficient;
    double liftCoefficient;
    double atmosphericDensity;
    double atmosphericPressure;
    double dragforce;
    double liftforce;
    double calcliftforce (int altitude);
    double calcdragforce (int altitude);
    double calcdragcoefficient (int altitude);
    double calcliftcoefficient (int altitude);
    void renderFins(bool hasBoosters, fullrocket mrocket);
    aerodynamics();
};


class fullrocket {
public:
    std::string name;
    rocket rbody;
    nozzle rnozzle;
    stage rstaging;
    aerodynamics rfins;
    tip rpayload;
    double calcBoosterMass(void);
    double calcSurfaceArea(void);
};


//ROCKET CALCULATION METHODS:
extern double rocketdeltaveval (stage *rstage, int stage);
extern double rocketnozzleeval (nozzle rnozzle, int altitude);
extern double rocketdrageval (aerodynamics rdynamics, int altitude);
extern double rocketdcoefficienteval (aerodynamics rdynamics, int altitude);
extern double rocketlcoefficienteval (aerodynamics rdynamics, int altitude);