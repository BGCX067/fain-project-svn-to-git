#ifndef END_POINT_DOT_H
#define END_POINT_DOT_H

#include "vec2.h"

class EndPoint : public Vec2
{
 public:
  bool isSelected;
  bool isSelectable;

  EndPoint(GLint a,GLint b);
  EndPoint(GLint a[2]);
  EndPoint(EndPoint& a);
  EndPoint(const Vec2& a);
  inline EndPoint()
  {}

  void drawSingleVertex () const;
};

#endif
