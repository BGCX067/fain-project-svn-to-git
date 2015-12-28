#ifndef DRAW_ALGO_DOT_H
#define DRAW_ALGO_DOT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "vec2.h"
#include "endPoint.h"
#include "sortedList.h"

////////////////////////////////////////////

std::vector<Vec2> drawBresenham(Vec2 A,Vec2 B);

int findClosestVertex(std::vector<EndPoint> v,Vec2 pos);

int findClosestEdge(std::vector<EndPoint> v,Vec2 pos);

EndPoint computeMiddle(EndPoint A,EndPoint B);
void smooth(std::vector<EndPoint>& v,bool closed);
void smooth2(std::vector<EndPoint>& v,bool closed);

void computeAABB(EndPoint aabb[2],std::vector<EndPoint> v);

void updateAABB(EndPoint aabb[2],EndPoint newE);

void drawVectorVertex (std::vector<Vec2> v);

std::vector< std::vector<Vec2> > computeEdges (std::vector<EndPoint> v, bool closed);

void drawAABB (EndPoint aabb[2]);

void fillPolygonInAABB(EndPoint aabb[2],std::vector< std::vector<Vec2> > edges);

SortedList<unsigned int> getIntersectionOnLine(int line,std::vector< std::vector<Vec2> > edges);

#endif
