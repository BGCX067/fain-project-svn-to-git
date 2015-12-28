#include <iostream>
#include "vec2.h"



Vec2::Vec2(GLint a,GLint b):
  x(a),y(b)
{}
Vec2::Vec2(GLint a[2]):
  x(a[0]),y(a[1])
{}
Vec2::Vec2(Vec2& a):
  x(a.x),y(a.y)
{}
Vec2::Vec2(const Vec2& a):
  x(a.x),y(a.y)
{}


void Vec2::drawVertex () const
{
  glVertex2i(x,y);
}


void Vec2::shuffleXY()
{
  GLint tmp=this->x;
  this->x=this->y;
  this->y=tmp;
}

void Vec2::negateX()
{
  this->x=-(this->x);
}
void Vec2::negateY()
{
  this->y=-(this->y);
}


float Vec2::length(Vec2 v) const
{
  Vec2 vecDiff=*this-v;
  return sqrt((float)vecDiff.dot(vecDiff));
}

int Vec2::dot (Vec2 v) const
{
  return this->x*v[0]+this->y*v[1];
}


// project on to segment AB
Vec2 Vec2::project (Vec2 A,Vec2 B) const
{
  Vec2 AB=B-A;
  Vec2 AP=(*this)-A;
  float distAB=AB.length();
  int k=AP.dot(AB)/distAB;
  Vec2 ret;
  // we clamp distance such that projection stays on segment and not on the whole line
  std::cout<<"k: "<<k<<" distAB: "<<distAB<<std::endl;
  if (k<0)
    {
      ret=A;
    }
  else if (k>distAB)
    {
      ret=B;
    }
  else
    {
      std::cout<<"calculus!"<<std::endl;
      // normalize x and y
      // can't be int cause values lie between 0 and 1
      float xNorme=(float)AB[0]/distAB;
      float yNorme=(float)AB[1]/distAB;

      xNorme*=(float)k;
      yNorme*=(float)k;

      ret=Vec2((int)xNorme+A[0],(int)yNorme+A[1]);
    }
  return ret;
}

// normalize against norme
float Vec2::normalize(GLint norme)
{
  float length=(*this).length()/(float)norme;
  (*this)/=((int)length);
  return length;
}
/////////////////////  ///////////////////// 
///////////////////// OVERRIDING OPERATORS
/////////////////////  /////////////////////
Vec2& Vec2::operator=(const Vec2 &rhs) {

  // Only do assignment if RHS is a different object from this.
  if (this != &rhs) {
    this->x=rhs.x;
    this->y=rhs.y;
  }

  return *this;
}

Vec2 & Vec2::operator+=(const Vec2 &rhs)
{
  this->x+=rhs.x;
  this->y+=rhs.y;
  return *this;
}
Vec2 & Vec2::operator-=(const Vec2 &rhs)
{
  this->x-=rhs.x;
  this->y-=rhs.y;
  return *this;
}
Vec2 & Vec2::operator*=(const GLint &rhs)
{
  this->x*=rhs;
  this->y*=rhs;
  return *this;
}

Vec2 & Vec2::operator/=(const GLint &rhs)
{
  this->x/=rhs;
  this->y/=rhs;
  return *this;
}

// Add this instance's value to other, and return a new instance
// with the result.
const Vec2 Vec2::operator+(const Vec2 &other) const 
{
  Vec2 result = *this;     // Make a copy of myself.  Same as MyClass result(*this);
  result += other;            // Use += to add other to the copy.
  return result;              // All done!
}
const Vec2 Vec2::operator-(const Vec2 &other) const 
{
  Vec2 result = *this;     // Make a copy of myself.  Same as MyClass result(*this);
  result -= other;            // Use -= to add other to the copy.
  return result;              // All done!
}

const Vec2 Vec2::operator/(const GLint &rhs) const
{
  Vec2 result = *this;
  result/=rhs;
  return result;
}
const Vec2 Vec2::operator*(const GLint &rhs) const
{
  Vec2 result = *this;
  result*=rhs;
  return result;
}


int& Vec2::operator[] (const int nIndex)
{
  switch(nIndex)
    {
    case 0:
      {
	return this->x;
      }
      break;
    case 1:
      {
	return this->y;
      }
      break;
    default:
      {
	std::cerr<<"Vec2: index out of range: "<<nIndex<<std::endl;
	return this->x;
      }
      break;
    }
}

std::ostream& operator<< (std::ostream& stream, const Vec2& v)
{
  stream<<"("<<v.x<<","<<v.y<<")";
  return stream;
}

int clamp (int a,int min, int max)
{
  int tmp=a;
  if (a<min)
    a=min;
  else if (a>max)
    a=max;
  if (tmp!=a)
    std::cout<<"clamped value"<<std::endl;
  return a;
}
