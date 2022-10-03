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

  double degree;
  std::cout << "Dx: " << dx << "   Dy: " << dy << std::endl;

  if (dx == 0) {
    if (dy > 0)
      degree = 0;
    else
      degree = 180;
  } 
  else if (dy == 0) {
    if (dx > 0)
      degree = 90;
    else
      degree = 90;
  } 
  else {
    degree = ((180 * atan(dy/dx)) / PI);

    if(degree > 0)
      degree = 90 - degree;
    else
      degree += 90;

    if (dy < 0)
      degree += (degree < 0 ? 180 : -180);
  }

  return degree;
}

double Points::angleTo(Points& other) {
  return Points::angleTo(other.x, other.y);
}
