#include <math.h>
#include <iostream>
#include "rocketcalc.hpp"
#include "figureslib.hpp"
#include "metrics.hpp"
//void setnozzleforce (double fnozzle); is in charge of setting the nozzle force attribute
void nozzle::setnozzleforce(double fnozzle) {
    nozzleforce = fnozzle;
    return;
}
/* double calcnozzleforce(void); is in charge of taking the other attributes
 * of the nozzle and calculating Fnozzle or the force of the nozzle as it exhausts*/
double nozzle::calcnozzleforce() {
    //Fnozzle = (Pe - Po)(A sub e in change)
    double pressurediff;
    pressurediff = exhaustpressure - outsidepressure;
    setnozzleforce(pressurediff * exhaustarea);
    return (nozzleforce);
}
/* double calcthrustforce(void); in nozzle class is in charge of using nozzle info
 * like surface area of nozzle exhaust, exit pressure, external pressure, to calculate
 * Fthrust of the entire rocket. Also takes parent attributes into account but focuses
 * primarily on nozzle factors (is the point)*/
double nozzle::calcthrustforce() {
    //Fthrust = (dm/dt)(dv sub e / dt) + (Pe - Po)(A sub e in change)
    double massflowproduct;
    massflowproduct = massflowrate * exhaustvelocity;
    calcnozzleforce();
    thrustforce = massflowproduct + nozzleforce;
    return (thrustforce);
}
/* double calctotalmass(void); in the staging class is in charge of collecting propellant,
 * structural mass, and payload mass data and returning a total mass.*/
double stage::calctotalmass(int stage) {
    double sum = 0;
    for(int i = stage; i < 8; i++) {
        sum += propellantmass[i];
        sum += structuremass[i];
    }
    totalmass[stage] = sum + payloadmass;
    return (sum);
}
/* double calcinitialmass(int stage); in the staging class is specific to a stage number
 * when calculating initial mass (usually used for mass ratio related calculations later on). */
double stage::calcinitialmass(int stage) {
    double sum = 0;
    if(totalmass[stage] == 0)
        calctotalmass(stage);
    initialmass = totalmass[stage];
    //std::cout << "STAGE " << stage+1 << " INITIAL MASS: " << initialmass << std::endl << std::endl;
    return initialmass;
}
/* double calcfinalmass(int stage); in the staging class is specific to a stage number
 * when calculating final mass (usually used for mass ratio related calculations later on). */
double stage::calcfinalmass(int stage) {
    double sum = 0;
    if(totalmass == 0)
        calctotalmass(stage);
    for (int i = stage; i < 8; i++) {
        sum += structuremass[i];
        //IMPORTANT: If factoring in propellant masses for delta-v or mass ratio calculations, uncomment the next line
        //sum += (i > stage) ? propellantmass[i] : 0;
    }
    finalmass = sum + payloadmass;
    //std::cout << "STAGE " << stage+1 << " FINAL MASS: " << finalmass << std::endl << std::endl;
    return finalmass;
}
/* double calcmassratio(int stage); in the staging class applies final mass and initial mass
 * functions to calculate a mass ratio specific to a certain stage of the rocket. */
double stage::calcmassratio(int stage) {
    //MR = Mfull/Mempty
    double mrNumerator, mrDenominator;
    mrNumerator = calcinitialmass(stage);
    mrDenominator = calcfinalmass(stage);
    //std::cout << mrNumerator << " " << mrDenominator << std::endl;
    massratio[stage] = mrNumerator/mrDenominator;
    return (massratio[stage]);
}

/* double calcdeltav(int stage); in the staging class gets specific stage number and applies it
 * to mass-ratio based functions while factoring in equivalent exhaust velocity to get the delta-v
 * (essential for maneuvers) of a certain stage.*/
double stage::calcdeltav(int stage) {
    //Deltav = gIsp log sub e (MR)
    double mrlog;
    exhaustvelocity = GRAVITY * specificimpulse;
    if(massratio[stage] == 0)
        calcmassratio(stage);
    //std::cout << "MASS RATIO, STAGE " << stage << " : " << massratio[stage] << std::endl;
    mrlog = log(massratio[stage]);
    deltav[stage] = exhaustvelocity * mrlog;
    return (deltav[stage]);
}

/* void renderStaging(int stagingtype, rocket mrocket); in the staging class uses a staging type
 * (one of the preprocessors listed in the rocketcalc.h file) and a rocket object to render a rocket's
 * stages. Note that the rocket object is only passed for the staging to extract coordinates pertaining
 * to a rocket body, and that it should remain seperate from the rocket object that the current staging object
 * extends.
 */
void stage::renderStaging(int stagingtype, rocket mrocket) {
    double heightpos;
    double portionfactor;
    if (stagingtype == STAGINGTYPESERIAL) {
        for (int n = 1; n < 8; n++) {
            if (totalmass[n] != 0) {
                portionfactor = totalmass[n] / mrocket.accuratebodymass;
                //std::cout << "TOTAL MASS: " << totalmass[n] << " ACCURATE BODY MASS: " << mrocket.accuratebodymass << " PORTION FACTOR: " << portionfactor << std::endl;
                heightpos = (mrocket.height / (n + 1)) + (mrocket.height * portionfactor);
                cylinder segment1(mrocket.radius * METER, (2.0f)*METER, (mrocket.rcylinder.xcoord),
                    (mrocket.rcylinder.ycoord), (mrocket.rcylinder.zcoord + (mrocket.height * METER)) - ((heightpos)*METER), mrocket.rcylinder.vertexnum);
                segment1.drawCylinder(100, 100, 100, GL_TRIANGLE_STRIP);
            }
        }
    }
    if (stagingtype == STAGINGTYPEPARALLEL) {
        for (int n = 1; n < 8; n++) {
           if(totalmass[n] != 0) {
                portionfactor = totalmass[n] / mrocket.accuratebodymass;
                heightpos = (mrocket.height/(n+1)) + (mrocket.height*portionfactor);
                {
                    cylinder booster1(mrocket.radius * METER, (heightpos)*METER, (mrocket.rcylinder.xcoord + (mrocket.radius * 1.5) * n),
                                            (mrocket.rcylinder.ycoord), (mrocket.rcylinder.zcoord+(mrocket.height*METER))-((heightpos)*METER), mrocket.rcylinder.vertexnum);
                    cone boosterpayload1(mrocket.radius * METER, 1.0f, (mrocket.rcylinder.xcoord + (mrocket.radius*1.5) * n), 
                                            (mrocket.rcylinder.ycoord), (mrocket.rcylinder.zcoord+(mrocket.height*METER))-((heightpos)*METER) - 1.0f,
                                            mrocket.rcylinder.vertexnum);
                    cone nozzle(2.0f * METER, 3.0f * METER, booster1.xcoord, booster1.ycoord, booster1.zcoord + heightpos*METER, 360);
                    nozzle.drawCone(100, 100, 100, GL_TRIANGLE_STRIP);
                    //These values are only default and experimental
                    boosterheight[n] = 1.0f;
                    boosterradius[n] = mrocket.radius;
                    //Set vertex amount
                    booster1.vertexnum = 32;
                    boosterpayload1.vertexnum = 32;
                    //First, render the solid booster
                    booster1.drawCylinder(100, 100, 100, GL_TRIANGLE_STRIP);
                    boosterpayload1.drawCone(100, 100, 100, GL_TRIANGLE_STRIP);
                    //Then, for the wireframe effect increase the radius
                    booster1.radius *= 1.03;
                    boosterpayload1.radius *= 1.03;
                    //Render again
                    boosterpayload1.drawCone(200, 200, 200, GL_LINES);
                    booster1.drawCylinder(200, 200, 200, GL_LINES);
                    cylinder booster2(mrocket.radius * METER, (heightpos)*METER, (mrocket.rcylinder.xcoord - (mrocket.radius * 1.5) * n),
                        (mrocket.rcylinder.ycoord), (mrocket.rcylinder.zcoord+ (mrocket.height * METER))-((heightpos)*METER), mrocket.rcylinder.vertexnum);
                    cone boosterpayload2(mrocket.radius * METER, 1.0f, (mrocket.rcylinder.xcoord - (mrocket.radius * 1.5) * n),
                        (mrocket.rcylinder.ycoord), (mrocket.rcylinder.zcoord+ (mrocket.height * METER))-((heightpos)*METER) - 1.0f,
                        mrocket.rcylinder.vertexnum);
                    cone nozzle2(2.0f * METER, 3.0f * METER, booster2.xcoord, booster2.ycoord, booster2.zcoord + heightpos*METER, 360);
                    nozzle2.drawCone(100, 100, 100, GL_TRIANGLE_STRIP);
                    //Set vertex amount
                    booster2.vertexnum = 32;
                    boosterpayload2.vertexnum = 32;
                    //First, render the solid booster
                    booster2.drawCylinder(100, 100, 100, GL_TRIANGLE_STRIP);
                    boosterpayload2.drawCone(100, 100, 100, GL_TRIANGLE_STRIP);
                    //Then, for the wireframe effect increase the radius
                    booster2.radius *= 1.03;
                    boosterpayload2.radius *= 1.03;
                    //Render again
                    boosterpayload2.drawCone(200, 200, 200, GL_LINES);
                    booster2.drawCylinder(200, 200, 200, GL_LINES);
                }

           }
        }
    }
}
/* stage::stage() is staging constructor */
stage::stage() {
    for(int index = 0; index < 8; index++) {
        structuremass[index] = 0;
        propellantmass[index] = 0;
        massratio[index] = 0;
    }
    stagenum = 0;
    payloadmass = 0;
}

double fullrocket::calcBoosterMass(void) {
    double total = 0;
    double heightpos = 0;
    double portionfactor = 0;
    for (int n = 0; n < 8; n++) {
        portionfactor = rstaging.totalmass[n] / rbody.accuratebodymass;
        heightpos = rbody.height * portionfactor;
        total += heightpos;
    }
    return total;
}

double fullrocket::calcSurfaceArea(void) {
    double tmparea = 0.0f;
    double circ = 0.0f;
    double length = 0.0f;
    double rbodyarea = 0.0f;
    //payload/tip
    circ = 2 * PI * rpayload.radius;
    tmparea = 4 * PI * pow(rpayload.radius, 2);
    rbodyarea += tmparea;
    //boosters
    for (int n = 1; n < 8; n++) {
        if (rstaging.totalmass[n] != 0) {
            circ = PI * rstaging.boosterradius[n];
            rbodyarea += 2 * (sqrtf(pow(rstaging.boosterheight[n], 2) + pow(rstaging.boosterradius[n], 2))) * circ;
        }
    }
    //fins
    std::cout << "FIN AREA: " << 2 * (sqrtf(pow(rfins.finheight, 2) + pow(rfins.finwidth, 2)) * rfins.finthickness) << std::endl;
    rbodyarea += 2 * (sqrtf(pow(rfins.finheight, 2) + pow(rfins.finwidth, 2)) * rfins.finthickness);
    rfins.liftcomponentArea = rbodyarea;
    return rbodyarea;
}

/* Aerodynamics class*/
double aerodynamics::calcdragcoefficient(int altitude) {
    double q;
    double atmosNumerator, atmosDenominator;
    if(atmosphericDensity == 0)
        atmosphericDensity = (altitude*AVERAGEDENSITYSLOPE)+1.2;
    std::cout << "Q: " << (averagevelocity * averagevelocity) / 2 * atmosphericDensity << std::endl;
    q = (averagevelocity * averagevelocity) / 2 * atmosphericDensity;
    atmosDenominator = (liftcomponentArea*q);
    atmosNumerator = (dragforce);
    dragCoefficient = atmosNumerator / atmosDenominator;
    return dragCoefficient;
}

double aerodynamics::calcliftcoefficient(int altitude) {
    double q;
    double atmosNumerator, atmosDenominator;
    if (atmosphericDensity == 0)
        atmosphericDensity = (altitude * AVERAGEDENSITYSLOPE) + 1.2;
    std::cout << "Q: " << (averagevelocity * averagevelocity) / 2 * atmosphericDensity << std::endl;
    q = (averagevelocity * averagevelocity) / 2 * atmosphericDensity;
    atmosDenominator = (liftcomponentArea * q);
    atmosNumerator = (liftforce);
    liftCoefficient = atmosNumerator / atmosDenominator;
    return liftCoefficient;
}

double aerodynamics::calcdragforce(int altitude) {
    double dragAreaProduct;
    if(atmosphericDensity == 0)
        atmosphericDensity = (altitude*AVERAGEDENSITYSLOPE)+1.2;
    if(dragCoefficient == 0)
        return (-1);
    dragAreaProduct = dragCoefficient * liftcomponentArea;
    atmosphericPressure = (atmosphericDensity * (averagevelocity*averagevelocity)) / 2;
    dragforce = atmosphericPressure * dragAreaProduct;
    return dragforce;
}

double aerodynamics::calcliftforce(int altitude) {
    double liftAreaProduct;
    if(atmosphericDensity == 0)
        atmosphericDensity = (altitude*AVERAGEDENSITYSLOPE)+1.2;
    if(dragCoefficient == 0)
        return (-1);
    liftAreaProduct = liftCoefficient * liftcomponentArea;
    atmosphericPressure = (atmosphericDensity * (averagevelocity*averagevelocity)) / 2;
    liftforce = atmosphericPressure * liftAreaProduct;
    return liftforce;
}

void aerodynamics::renderFins(bool hasBoosters, fullrocket mrocket) {
    double boosterxincrement = 0;
    double rightx, rightz, leftx, leftz;
    if (hasBoosters) {
        for (int n = 1; n < 8; n++)
            if (mrocket.rstaging.totalmass[n] != 0)
                boosterxincrement += mrocket.rbody.rcylinder.radius;
    }
    rightx = rightz = leftx = leftz = 0;
    rightx = mrocket.rbody.rcylinder.xcoord - mrocket.rbody.rcylinder.radius - boosterxincrement;
    leftx = mrocket.rbody.rcylinder.xcoord + mrocket.rbody.rcylinder.radius + boosterxincrement;
    rightz = (mrocket.rbody.rcylinder.zcoord + mrocket.rbody.rcylinder.height);
    leftz = (mrocket.rbody.rcylinder.zcoord + mrocket.rbody.rcylinder.height);
    twinfins[0].xcoord = rightx;
    twinfins[0].height = finthickness * METER;
    twinfins[0].horizlength = -finwidth * METER;
    twinfins[0].vertlength = -finheight * METER;
    twinfins[0].ycoord = -mrocket.rbody.rcylinder.ycoord;
    twinfins[0].zcoord = rightz;
    twinfins[1].xcoord = leftx;
    twinfins[1].height = finthickness * METER;
    twinfins[1].horizlength = finwidth * METER;
    twinfins[1].vertlength = -finheight * METER;
    twinfins[1].ycoord = -mrocket.rbody.rcylinder.ycoord;
    twinfins[1].zcoord = leftz;
    twinfins[0].drawPrism(100, 100, 100, GL_TRIANGLE_STRIP);
    twinfins[1].drawPrism(100, 100, 100, GL_TRIANGLE_STRIP);
}

/* Aerodynamic force constructor*/
aerodynamics::aerodynamics() {
    atmosphericDensity = 0;
    dragCoefficient = 0;
    liftCoefficient = 0;
    liftcomponentArea = 0;
    atmosphericPressure = 0;
}

/* double calcmassratio(void); of the rocket class uses general initial and final mass info to
 * get a mass ratio in a very basic way. As you can see, the calcmassratio function is polymorphic
 * and is more specific in other parts of the rocket, i.e when it comes to staging.*/
double rocket::calcmassratio() {
    //No safety check.
    massratio = initialmass / finalmass;
    return (massratio);
}

double rocket::calcimpulse (int derivation) {
    double tempproduct;
    if(dMpropellant == 0)
        calcchangepropellant(derivation);
    switch (derivation) {
        case SPECIFICIMPULSEDERIVATION:
            tempproduct = dMpropellant * GRAVITY;
            impulse = specificimpulse * tempproduct;
            break;
        case VELOCITYDERIVATION:
            tempproduct = dMpropellant * exhaustvelocity;
            impulse = tempproduct;
            break;
    }
    return (impulse);
}

double rocket::calcchangepropellant (int derivation) {
    double tempquotient;
    switch (derivation) {
        case SPECIFICIMPULSEDERIVATION:
            tempquotient = impulse / (specificimpulse*GRAVITY);
            dMpropellant = specificimpulse * tempquotient;
            break;
        case VELOCITYDERIVATION:
            tempquotient = impulse / exhaustvelocity;
            dMpropellant = tempquotient;
            break;
    }
    return (dMpropellant);
}

double rocket::calcthrustforce(int derivation) {
    double equiv;
    if(impulse == 0)
        calcimpulse(derivation);
    if(massflowrate == 0)
        return (-1);
    equiv = impulse / dMpropellant;
    thrustforce = massflowrate * equiv;
    return thrustforce;
}

void rocket::renderRocketCylinder(GLenum primtype) {
    double rvolume;
    double toparea;
    rvolume = totalbodymass / material;
    //std::cout << rvolume << std::endl;
    if (radius == 0) {
        if (height != 0) {
            radius = height * 0.2;
        }
    }
    else if (height == 0) {
        toparea = PI * (radius * radius);
        height = rvolume / toparea;
        //std::cout << height << std::endl;
    }
    passMeasurements();
    rcylinder.drawCylinder(190, 190, 190, primtype);
    radius *= 1.03;
    rcylinder.vertexnum = 32;
    rcylinder.drawCylinder(200, 200, 200, GL_LINES);
}

void rocket::calcRocketMass(void) {
    double rvolume = (PI * (radius * radius) * height);
    totalbodymass =  rvolume * material * hullthickness;
    //std::cout << radius << " " << height << " " << rvolume << " " << totalmass << std::endl;
    passMeasurements();
}

/* void calcBodyHeight(void); is designed to get the height of the rocket's main body, but it does 
 * not function perfectly yet due to how a rocket would not naturally have a perfectly cylindrical shape,
 * as much of the library assumes right now, but with the right totalbodymass (HULL MASS) you can get accurate results.*/
void rocket::calcBodyHeight(void) {
    double rvolume;
    double toparea;
    rvolume = totalbodymass / material;
    toparea = PI * (radius * radius);
    height = rvolume / toparea;
}

//For metric purposes
void rocket::passMeasurements(void) {
    rcylinder.radius = radius * METER;
    rcylinder.height = height * METER;
}

rocket::rocket(double mtotal, double mdot, double dMprop, double i, double isp, double fthrust, double c,
               double minitial, double mfinal, double mr) {
    totalbodymass = mtotal;
    massflowrate = mdot;
    dMpropellant = dMprop;
    impulse = i;
    specificimpulse = isp;
    thrustforce = fthrust;
    exhaustvelocity = c;
    initialmass = minitial;
    finalmass = mfinal;
    massratio = mr;
    averagevelocity = 0;
    height = 0;
    radius = 0;
}

//This is a sample design for now, looks similar to the spacex payload encasing
void tip::renderRocketTip(GLenum primtype, rocket body) {
    radius = (body.rcylinder.radius) * 1.45;
    tipbody = cylinder(body.rcylinder.radius * 1.45, height * METER, body.rcylinder.xcoord, body.rcylinder.ycoord, body.rcylinder.zcoord, 60);
    sphere tipsphere1(tipbody.xcoord, -tipbody.ycoord, tipbody.zcoord, tipbody.radius, 60, 60);
    sphere tipsphere2(tipbody.xcoord, -tipbody.ycoord, tipbody.zcoord + (height * METER), tipbody.radius, 60, 60);
    tipbody.drawCylinder(150, 150, 150, GL_TRIANGLE_STRIP);
    tipbody.radius *= 1.01;
    tipbody.drawCylinder(200, 200, 200, GL_LINES);
    tipsphere1.rendersphere(150, 150, 150, GL_POLYGON);
    tipsphere2.rendersphere(150, 150, 150, GL_POLYGON);
    tipsphere1.stacks = 32;
    tipsphere1.sectors = 32;
    tipsphere2.stacks = 32;
    tipsphere2.sectors = 32;
    tipsphere1.rendersphere(200, 200, 200, GL_LINE_STRIP);
    tipsphere2.rendersphere(200, 200, 200, GL_LINE_STRIP);
}