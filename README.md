# RocketGL
Basic rocket physics library, operates on the basis of polymorphism across different rocket component sub-classes, with the evaluation of rocket variables varying depending on what the focus component is (since different components factor in different variables).
## Rendering
Uses a custom figure rendering library to render 3d figures with GLUT, rocket component rendering is done in class libraries
## To-do
Many of the functions of the rocket classes are not yet implemented, those will be added with time, this is just a first version
Note: main.cpp is just an implementation, not a part of the library
# A Basic Demo:

To start rendering and modeling a rocket, the first thing you need to do is initialize a ```fullrocket``` object:
```cpp
fullrocket myrocket;
```
Though you can technically initialize with a constructor, it is highly recommended that you set variables afterwards to truncate and simplify.
Now the fullrocket class is defined as such:
```cpp
class fullrocket {
public:
    std::string name;
    rocket rbody;
    nozzle rnozzle;
    stage rstaging;
    aerodynamics rfins;
    tip rpayload;
    double calcBoosterMass(void);
};
```
Where you have all the other rocket component sub-classes loaded into one class to house and make it easier to organize all your rocket data.
Now note that any and all functions in the fullrocket class will be there for the purpose of inter-component calculations. For now, let's focus
on the ```rbody``` object. This object should be your MAIN rocket body object, even though the other objects include the rocket class as a parent class
as well. Why? Well the function of the subclasses is to mainly stick with their own variables and members, but they have the rocket class as a parent
because the subclass will occasionally use broader variables that belong in a broader, more general rocket class (for example, exhaust velocity and thrust would be in the rocket class, and the staging sub-class would on occasion borrow those for individual calculations, while the staging class members still remain specific).
Now, let's say we want to get a rocket cylinder/body ready for rendering or modeling, we would do it something like this:
```cpp
fullrocket myrocket;
//Let's set some basic variables
myrocket.rbody.height = 70 //in METERS, we are always going by the metric system
myrocket.rbody.radius = 1.88f //again, meters
myrocket.rbody.hullthickness = 0.0047 //hull thickness is used for more accurately calculating volume or density, but it also represent's hull thickness in meters
myrocket.rbody.totalbodymass = 51000 //hull mass, NOT the total mass for the rocket, be VERY CAREFUL about this
myrocket.rbody.accuratebodymass = 1420788 //THIS is the accurate total mass for the rocket, in kg
myrocket.rbody.material = STAINLESSSTEEL //defined in metrics.hpp
myrocket.rbody.rcylinder.xcoord = -35.0f * METER; //Just a sample coordinate, but it does implement the METER unit, which you should do too
myrocket.rbody.rcylinder.ycoord = -20.0f * METER;
myrocket.rbody.rcylinder.zcoord = -3.0f * METER;
myrocket.rbody.rcylinder.vertexnum = 90; //Sharpness of the rocket body cylinder. Basically the more you have, the smoother it will render
```
Now we have a fully minimal rocket body set for rendering.
Docs will be finished later...
![DemoModel](./Images/demo1.PNG)
