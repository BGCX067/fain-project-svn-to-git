#ifndef VEC_2_DOT_H
#define VEC_2_DOT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <math.h>

class Vec2
{
  friend std::ostream& operator<< (std::ostream& stream, const Vec2& v) ;

 public :
  GLint x,y;
  inline Vec2() {};
  Vec2(GLint a,GLint b);
  Vec2(GLint a[2]);
  Vec2(Vec2& a);
  Vec2(const Vec2& a);

  /////////////////////  ///////////////////// 
  ///////////////////// METHODS
  /////////////////////  /////////////////////

  virtual void drawVertex () const;

  void shuffleXY();


  void negateX();

  void negateY();

  /* length of a vector to another one, (0,0) is default */
  float length(Vec2 v=Vec2(0,0)) const;

  /* dot product */
  int dot (Vec2 v) const;

  /* normalize vector and return old length */
  float normalize(GLint norme=100);

  /* return projection of (*this) on AB */
  Vec2 project(Vec2 A,Vec2 B) const; 

  /////////////////////  ///////////////////// 
  ///////////////////// OVERRIDING OPERATORS
  /////////////////////  /////////////////////
  Vec2& operator=(const Vec2 &rhs);

  Vec2 & operator+=(const Vec2 &rhs);

  Vec2 & operator-=(const Vec2 &rhs);

  Vec2 & operator*=(const GLint &rhs);

  Vec2 & operator/=(const GLint &rhs);

  // Add this instance's value to other, and return a new instance
  // with the result.
  const Vec2 operator+(const Vec2 &other) const ;

  const Vec2 operator-(const Vec2 &other) const ;

  const Vec2 operator/(const GLint &rhs) const;

  const Vec2 operator*(const GLint &rhs) const;

  int& operator[] (const int nIndex) ;

};


int clamp (int a,int min, int max);
#endif
