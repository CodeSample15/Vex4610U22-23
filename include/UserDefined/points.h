#pragma once

//to make my life easier when it comes to position tracking
class Points
{
  public:
    Points();
    Points(double x, double y);

    double x;
    double y;

    Points vectorTo(double x, double y); //to get a normalized vector that points in the direction of one point from the x y values in the class
    Points vectorTo(Points& other);

    double distanceTo(double x, double y); //pretty self-explanatory
    double distanceTo(Points& other);

    int angleTo(double x, double y); //for determining what angle the robot needs to turn to in order to point towards a specific point
    int angleTo(Points& other);
};
