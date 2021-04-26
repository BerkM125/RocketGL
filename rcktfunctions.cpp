#include <stdio.h>
#include <stdlib.h>
#include "rocketcalc.hpp"
#include "barometric.h"

/*Take deltav and rocket structure mass requirements to calculate rocket's build needs*/
//REQUIRES: staging, structure mass, payload mass, some other stuff
double rocketdeltaveval (stage *rstage, int stage) {
    double forceProduct;
    rstage->calcmassratio(stage);
    rstage->calcdeltav(stage);
    rstage->calctotalmass(stage);
    rstage->exhaustvelocity = (GRAVITY * rstage->specificimpulse);
    printf("DELTA V, STAGE %d: %f\n", stage+1, rstage->deltav[stage]);
    return rstage->deltav[stage];
}
/*Take nozzle area and force specifications to calculate pressure change needed to achieve accurate amounts of thrust.*/
//REQUIRES: Nozzle area, thrust needed
double rocketnozzleeval (nozzle rnozzle, int altitude) {
    struct Formulae barometrics;
    double thrustAreaQuotient, externalPressure;
    thrustAreaQuotient = (rnozzle.thrustforce / rnozzle.exhaustarea);
    externalPressure = PASCAL_TO_PSI(barometrics.barometricPressure(altitude));
    rnozzle.outsidepressure = externalPressure;
    rnozzle.exhaustpressure = thrustAreaQuotient + rnozzle.outsidepressure;
    rnozzle.pressurechange = thrustAreaQuotient;
    printf("EXTERNAL PRESSURE: %f PSI\n", rnozzle.outsidepressure);
    printf("EXHAUST PRESSURE: %f PSI\n", rnozzle.exhaustpressure);
    printf("PRESSURE CHANGE NEEDED: %f PSI\n", rnozzle.pressurechange);
    return (rnozzle.pressurechange);
}
/*Take rocket fin and surface area info, altitude (possibly atmospheric density), velocity,
 * and drag coefficient to calculate the drag on the rocket on its trajectory.*/
double rocketdrageval (aerodynamics rdynamics, int altitude) {
    rdynamics.calcdragforce(altitude);
    printf("DRAG FORCE AT %d METERS: %f NEWTONS\n", altitude, rdynamics.dragforce);
    return rdynamics.dragforce;
}
/*Take rocket fin and surface area info, altitude (possibly atmospheric density), velocity,
 * and drag coefficient to calculate the drag on the rocket on its trajectory.*/
double rocketlifteval (aerodynamics rdynamics, int altitude) {
    rdynamics.calcliftforce(altitude);
    printf("LIFT FORCE AT %d METERS: %f NEWTONS\n", altitude, rdynamics.dragforce);
    return rdynamics.liftforce;
}
/*Take rocket fin and surface area info, altitude (possibly atmospheric density), velocity,
 * and drag force to calculate the drag coefficient acting on the rocket on its trajectory.*/
double rocketdcoefficienteval (aerodynamics rdynamics, int altitude) {
    rdynamics.calcdragcoefficient(altitude);
    printf("DRAG COEFFICIENT AT %d METERS: %f\n", altitude, rdynamics.dragCoefficient);
    return rdynamics.dragCoefficient;
}
/*Take rocket fin and surface area info, altitude (possibly atmospheric density), velocity,
 * and lift force to calculate the lift coefficient actig on the rocket on its trajectory.*/
double rocketlcoefficienteval (aerodynamics rdynamics, int altitude) {
    rdynamics.calcliftcoefficient(altitude);
    printf("LIFT COEFFICIENT AT %d METERS: %f\n", altitude, rdynamics.liftCoefficient);
    return rdynamics.liftCoefficient;
}