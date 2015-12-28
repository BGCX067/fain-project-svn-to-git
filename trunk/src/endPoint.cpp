#include "endPoint.h"

EndPoint::EndPoint(GLint a,GLint b):
  Vec2(a,b)
{
  isSelected=false;
  isSelectable=false;
}
EndPoint::EndPoint(GLint a[2]):
  Vec2(a)
{
  isSelected=false;
  isSelectable=false;
}
EndPoint::EndPoint(EndPoint& a):
  Vec2(a)
{
  isSelected=false;
  isSelectable=false;
}
EndPoint::EndPoint(const Vec2& a):
  Vec2(a)
{
  isSelected=false;
  isSelectable=false;
}

void EndPoint::drawSingleVertex () const
{
  float rayon=10;
  if (isSelected)
    {
      /* draw a red square */
      glColor3f(1.,0.,0.);
    }
  else if (isSelectable)
    {
      /* draw a less-red square */
      glColor3f(0.7,0.,0.);
    }
  glPointSize(rayon);

  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();

  glPointSize(1);
  glColor3f(0.,0.,0.);

}
