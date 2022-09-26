#include "UserDefined/points.h"
#include "UserDefined/methods.h"
#include <iostream>
#include <cmath>

Points::Points()
{
  Points::x = 0;
  Points::y = 0;
}

Points::Points(double x, double y)
{
  Points::x = x;
  Points::y = y;
}


Points Points::vectorTo(double x, double y)
{
  x = Points::x - x;
  y = Points::y - y;
  double vectorLength = sqrt((x*x) + (y*y));

  return Points(x/vectorLength, y/vectorLength);
}

Points Points::vectorTo(Points& other)
{
  return Points::vectorTo(other.x, other.y);
}


double Points::distanceTo(double x, double y) {
  return sqrt((x*x) + (y*y));
}

double Points::distanceTo(Points& other) {
  return Points::distanceTo(other.x, other.y);
}


double Points::angleTo(double x, double y) {
  double dx = x - Points::x;
  double dy = y - Points::y;

  double degree = (180 * atan(dy/dx)) / PI;
  std::cout << degree << std::endl;

  //figure out what quadrant the degree is in and add numbers accordingly
  if(dx > 0 && dy > 0) 
    return 90 - degree;    //degree is positive
  else if(dx < 0 && dy > 0)
    return -(90 + degree); //degree will be negative
  else if(dx < 0 && dy < 0)
    return degree - 90;    //degree is negative
  else
    return degree + 90;    //degree is positive
}

double Points::angleTo(Points& other) {
  return Points::angleTo(other.x, other.y);
}
