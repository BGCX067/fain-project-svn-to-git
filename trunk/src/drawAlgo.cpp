#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "drawAlgo.h"
#include "endPoint.h"

int drawBresenhamFirstOctant(std::vector<Vec2> & pixelVector,Vec2 A,Vec2 B)
{
  Vec2 currentPixel(A);
  Vec2 diffVector(B-A);
  int e=diffVector[0];
  diffVector*=2;

  while(currentPixel[0]<=B[0])
    {
      pixelVector.push_back(currentPixel);
      currentPixel.x++;
      // assignement and test with returned value
      if((e-=diffVector[1])<=0)
	{
	  currentPixel.y++;
	  e+=diffVector[0];
	}
    }
  return (int) diffVector[0];
}

std::vector<Vec2> drawBresenham(Vec2 A, Vec2 B)
{
  std::vector<Vec2> pixelVector;
  Vec2 diffVector(B-A);
  // translate A and B such that A is 0,0
  Vec2 newA=Vec2(0,0);
  Vec2 newB=diffVector;

  bool shuffleXY=false;
  bool negateY=false;
  bool negateX=false;

  if (diffVector[0]>0)
    {
      if (diffVector[1]>0)
	{
	  if (diffVector[0]<=diffVector[1])
	    {
	      // second octant
	      newB.shuffleXY();
	      shuffleXY=true;
	    }
	}
      else // diffY<0
	{
	  // negative diff Y=> negate Y in any case
	  newB[1]=-newB[1];
	  negateY=true;
	  // xslope < yslope
	  if (diffVector[0]<-diffVector[1])
	    {
	      // 7th octant: got to shuffle
	      newB.shuffleXY();
	      shuffleXY=true;
	    }
	}
    }
  else // diffX<0
    {
      // negative diff X=> negate X in any case
      newB[0]=-newB[0];
      negateX=true;
      if (diffVector[1]>0)
	{
	  // xslope < yslope
	  if (-diffVector[0]<diffVector[1])
	    {
	      // 3rd octant
	      // got to shuffle
	      newB.shuffleXY();
	      shuffleXY=true;
	    }
	}
      else // diffY<0
	{
	  newB[1]=-newB[1];
	  negateY=true;
	  // xslope < yslope
	  if (diffVector[0]>diffVector[1])
	    {
	      // 6th octant, got to shuffle
	      newB.shuffleXY();
	      shuffleXY=true;
	    }
	}
    }
  drawBresenhamFirstOctant(pixelVector,newA,newB);


  if (shuffleXY)
    {
      for (unsigned int i=0;i<pixelVector.size();i++)
	{
	  pixelVector[i].shuffleXY();
	}
    }
  // apply transformation
  if (negateX)
    {
      for (unsigned int i=0;i<pixelVector.size();i++)
	{
	  pixelVector[i].negateX();
	}
    }
  if (negateY)
    {
      for (unsigned int i=0;i<pixelVector.size();i++)
	{
	  pixelVector[i].negateY();
	}
    }

  // we used newA and newB, 
  // we have to translate the calculated pixels
  for (unsigned int i=0;i<pixelVector.size();i++)
    {
      pixelVector[i]+=A;
    }
  // done
  return pixelVector;
}

int findClosestVertex(std::vector<EndPoint> v,Vec2 pos)
{
  // case no points
  if (v.empty())
    return -1;

  float minDist=pos.length(v[0]);
  float tmpDist;
  int subscript=0;
  for (unsigned int i=1;i<v.size();i++)
    {
      tmpDist=pos.length(v[i]);
      if (tmpDist<minDist)
	{
	  minDist=tmpDist;
	  subscript=i;
	}
    }

  return subscript;
}

int findClosestEdge(std::vector<EndPoint> v,Vec2 pos)
{
  unsigned int vSize=v.size();
  // case no points
  if (vSize<2)
    return -1;


  Vec2 proj=pos.project(v[0],v[1]);
  float minDist=(pos).length(proj);
  float tmpDist;
  int subscript=0;

  for (unsigned int i=1;i<vSize;i++)
    {
      Vec2 proj=pos.project(v[i],v[((i+1)%vSize)]);
      tmpDist=pos.length(proj);
      if (tmpDist<minDist)
	{
	  minDist=tmpDist;
	  subscript=i;
	}
    }

  // got an small issue with edge subscript there
  return (subscript+1)%vSize;
}

EndPoint computeMiddle(EndPoint A,EndPoint B)
{
  return EndPoint((A+B)/2);
}

void smooth(std::vector<EndPoint>& v,bool closed)
{
  unsigned int vSize=v.size();
  if (vSize<2)
    return;
  std::vector<EndPoint> newV;
  for (unsigned int i=0;i<vSize-1;i++)
    {
      if (i%2==0)
	newV.push_back(v[i]);
      newV.push_back(computeMiddle(v[i],v[(i+1)]));
    }
  // do we have to push the last
  if ((vSize-1)%2==0)
    newV.push_back(v[vSize-1]);
    
  // if closed, compute the middle of last edge
  if (closed)
    newV.push_back(computeMiddle(v[vSize-1],v[0]));
  // put newV in the reference
  v=newV;
}

void smooth2(std::vector<EndPoint>& v,bool closed)
{
  unsigned int vSize=v.size();
  if (vSize<2)
    return;
  
  // if the shape isn't close we don't do the last edge
  unsigned int numberOfEdges=vSize;
  if (!closed)
    numberOfEdges--;
  std::vector<EndPoint> newV;
  for (unsigned int i=0;i<numberOfEdges;i++)
    {
      // for each edge, we add 2 vertices

      // one clode to the first endpoint
      newV.push_back((v[i]*3+v[(i+1)%vSize])/4);
      // one clode to the second endpoint
      newV.push_back((v[i]+v[(i+1)%vSize]*3)/4);
    }
  v=newV;
}

void computeAABB(EndPoint aabb[2],std::vector<EndPoint> v)
{
  aabb[0]=v[0];
  aabb[1]=v[0];
  for (unsigned int i=1;i<v.size();i++)
    {
      updateAABB(aabb,v[i]);
    }
}

void updateAABB(EndPoint aabb[2],EndPoint newE)
{
  // update AABB when adding one endpoint
  if (aabb[0].x>newE.x)
    {
      aabb[0].x=newE.x;
    }
  if (aabb[0].y>newE.y)
    {
      aabb[0].y=newE.y;
    }
  if (aabb[1].x<newE.x)
    {
      aabb[1].x=newE.x;
    }
  if (aabb[1].y<newE.y)
    {
      aabb[1].y=newE.y;
    }
}

std::vector< std::vector<Vec2> > computeEdges (std::vector<EndPoint> v, bool closed)
{
  std::vector< std::vector<Vec2> > edges;
  unsigned int vsize=v.size();
  for (unsigned int i=0;i<vsize-1;i++)
    {
      edges.push_back(drawBresenham(v[i],v[i+1]));
    }
  if (closed)
    {
      edges.push_back(drawBresenham(v[vsize-1],v[0]));
    }
  return edges;
}

void drawVectorVertex (std::vector<Vec2> v)
{
  glBegin(GL_POINTS);
  for (unsigned int j=0;j<v.size();j++)
    {
      v[j].drawVertex();
    }
  glEnd();
}

// draw the AABB, mostly for debugging purpose
void drawAABB (EndPoint aabb[2])
{
  std::vector<Vec2> pixels;
  EndPoint topRight(aabb[1].x,aabb[0].y);
  EndPoint bottomLeft(aabb[0].x,aabb[1].y);

  pixels=drawBresenham(aabb[0],topRight);
  drawVectorVertex(pixels);
  pixels.clear();

  pixels=drawBresenham(topRight,aabb[1]);
  drawVectorVertex(pixels);
  pixels.clear();

  pixels=drawBresenham(aabb[1],bottomLeft);
  drawVectorVertex(pixels);
  pixels.clear();

  pixels=drawBresenham(bottomLeft,aabb[0]);
  drawVectorVertex(pixels);
  pixels.clear();
}

void fillPolygonInAABB(EndPoint aabb[2],std::vector< std::vector<Vec2> > edges)
{
  // xi: X coordinate of intersections points between horizontal line and edges
  SortedList<unsigned int> xi;

  // for all the line in the AABB	
  for (int line=aabb[0].y;line<aabb[1].y;line++)
    {
      xi=getIntersectionOnLine(line,edges);
      if (xi.size()==0) continue;

      // we treat the 2 AABB extremity as bounding box (they'll be sorted in the list)
      xi.insert(aabb[0].x);
      xi.insert(aabb[1].x);

      // now we draw the pixels with respect to parity of number of intersection
      // the starting point depends whether we start inside or outside the polygon

      // do we start inside or outside the polygon?
      unsigned int counter=(xi.size()+1)%2;


      glColor3f(0.,0.,0.);
      glBegin(GL_POINTS);
      // we increment the counter 2 by 2
      for (;counter<xi.size()-1;counter=counter+2)
	{
	  for (unsigned int xCoord=xi[counter];xCoord<xi[counter+1];xCoord++)
	    {
	      glVertex2i(xCoord,line);
	    }
	}
      glEnd();
      xi.freeList();
      // one line is done
    }
}

SortedList<unsigned int> getIntersectionOnLine(int line,std::vector< std::vector<Vec2> > edges)
{
  SortedList<unsigned int> xi;
  // for all edges
  for (unsigned int i=0;i<edges.size();i++)
    {

      bool underneath=false;
      bool intersect=false;
      unsigned int xInter=0;
      // for all pixels of an edge
      for (unsigned int j=0;j<edges[i].size();j++)
	{
	  // if the line is underneath at some point
	  if (edges[i][j].y<line)
	    underneath=true;
	  else if ((edges[i][j].y==line)&&(!intersect))
	    {
	      // we push at most one point for an edge
	      intersect=true;
	      xInter=edges[i][j].x;
	    }
	}
      // we insert the pixel if and only if there is an intersection and the edge was underneath at some point 
      if ((intersect)&&(underneath))
	{
	  xi.insert(xInter);
	}
    }
  return xi;
}
