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
  double dx = Points::x - x;
  double dy = Points::y - y;

  return sqrt((dx*dx) + (dy*dy));
}

double Points::distanceTo(Points& other) {
  return Points::distanceTo(other.x, other.y);
}


double Points::angleTo(double x, double y) {
  double dx = x - Points::x;
  double dy = y - Points::y;

  double degree;
  std::cout << "Dx: " << dx << "   Dy: " << dy << std::endl;

  degree = ((180 * atan2(dy,dx)) / PI) + 90;
  return -degree;
}

double Points::angleTo(Points& other) {
  return Points::angleTo(other.x, other.y);
}
