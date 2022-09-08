#include "UserDefined/points.h"
#include "UserDefined/methods.h"
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


int Points::angleTo(double x, double y) {
  Points robot_pos = getPositionXY();

  double dx = x - robot_pos.x;
  double dy = y - robot_pos.y;

  return (int)atan(dx/dy);
}

int Points::angleTo(Points& other) {
  return Points::angleTo(other.x, other.y);
}
