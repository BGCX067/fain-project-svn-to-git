#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec2.h"
#include "endPoint.h"
#include "drawAlgo.h"

#define APPEND_MODE 0
#define VERTEX_MODE 1
#define EDGE_MODE 2


short int mode;
bool fillPolygon;
bool closePolygon;

int g_width=600;
int g_height=800;

std::vector<EndPoint> endPoints;
EndPoint aabb[2];
EndPoint mousePosition;

unsigned long int pointerOnSelectedVertex;

void initialiseGLState()
{
  glClearColor(0.75, 0.75, 0.75, 1.0);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, g_width, g_height, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);

}


void initScene()
{
  fillPolygon=false;
  closePolygon=false;
  mode=APPEND_MODE;
  pointerOnSelectedVertex=0;
  aabb[0]=EndPoint(0,0);
  aabb[1]=EndPoint(0,0);
}

// ensure that pointerOnSelectedVertex doesn't have a wrong value
void cropPointerOnSelectedVertex()
{
  if ((pointerOnSelectedVertex>=endPoints.size())
      ||(pointerOnSelectedVertex<0))
    pointerOnSelectedVertex=0;
}

////////////////////
// Callbacks GLUT //
////////////////////


/**
 * Callback appelé lorsqu'un rendu doit être effectué.
 */
void display(void)
{
  // clear screen
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.,0.,0.);
  
  std::vector< std::vector<Vec2> > edges;
  // exit the display if the endpoint vector is empty
  if (!endPoints.empty())
    {
      // we draw highlited endpoints
      for (unsigned int i=0;i<endPoints.size();i++)
	{
	  // the color of vertices is held within the EndPoint structure (isSelected)
	  endPoints[i].drawSingleVertex();
	}

      // compute edges
      edges=computeEdges(endPoints,closePolygon);

      for (unsigned int i=0;i<edges.size();i++)
	{
	  // edges[i] is pointed with pointer=i+1
	  // we hit the 0 with a modulus
	  if ((mode==EDGE_MODE)
	      &&
	      (pointerOnSelectedVertex == ((i+1)%endPoints.size())))
	    {
	      // red color if selected point
	      glColor3f(1.,0.,0.);
	    }
	  else
	    {
	      // black otherwhise
	      glColor3f(0.,0.,0.);
	    }
	  drawVectorVertex(edges[i]);
	}
      glColor3f(0.,0.,0.);

      if (closePolygon)
	{
	  computeAABB(aabb,endPoints);
	  drawAABB(aabb);
	  if (fillPolygon)
	    {
	      fillPolygonInAABB(aabb,edges);
	    }
	}

      // // draw line to the mouse enpoint
      // if (sizeOfEndPoints>0)
      //   {
      //     endPoints[sizeOfEndPoints-1].drawVertex();
      //     pixels=drawBresenham(endPoints[sizeOfEndPoints-1],mousePosition);
      //     // draw pixels
      //     glBegin(GL_POINTS);
      //     for (unsigned int i=0;i<pixels.size();i++)
      // 	{
      // 	  pixels[i].drawVertex();
      // 	}
      //     glEnd();
      //     pixels.clear();
      //   }
    }

  glutSwapBuffers();
}

/**
 * Callback appelé lorsque la fenêtre est redimensionnée.
 */
void reshape(GLint width, GLint height)
{
  g_width = width;
  g_height = height;

  glViewport(0, 0, g_width, g_height);

  // Demande de redessiner la fenêtre
  glutPostRedisplay();
}

/**
 * Callback appelé lorsqu'un clic souris est effectué.
 */
void mouseClicked(int button, int state, int x, int y)
{
  switch (mode)
    {
    case(APPEND_MODE):
      {
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	  {
	    EndPoint newEndpoint=EndPoint(x,y);
	    endPoints.push_back(newEndpoint);
	    std::cout<<"added endPoint "<<newEndpoint<<std::endl;
	  }
	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
	  {
	    if (!endPoints.empty()) {
	      endPoints.pop_back();
	      std::cout<<"removed endPoint"<<std::endl;
	    }
	  }
      }
    break;
    case(VERTEX_MODE):
      {
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	  {
	    endPoints[pointerOnSelectedVertex].isSelected=false;
	    pointerOnSelectedVertex=findClosestVertex(endPoints,Vec2(x,y));
	    endPoints[pointerOnSelectedVertex].isSelected=true;
	  }
      }
    break;
    case(EDGE_MODE):
      {
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	  {
	    endPoints[pointerOnSelectedVertex].isSelected=false;
	    pointerOnSelectedVertex=findClosestEdge(endPoints,Vec2(x,y));
	    endPoints[pointerOnSelectedVertex].isSelected=true;
	  }
      }
    break;
    }

  glutPostRedisplay();
}

/**
 * Callback appelé lorsque la souris a bougé dans la fenêtre.
 */
void mouseMove(int x, int y)
{
  mousePosition=Vec2(x,y);
  glutPostRedisplay();
}

/**
 * Callback appelé lorsque aucun événements n'est à traîter.
 */
void idle()
{
  glutPostRedisplay();
}

unsigned long int getPreviousSubscript()
{
  unsigned long int previousSubscript;
  if(pointerOnSelectedVertex==0)
    previousSubscript=endPoints.size()-1;
  else
    previousSubscript=pointerOnSelectedVertex-1;
  return previousSubscript;
}

void deletePointedVertex()
{
  endPoints.erase(endPoints.begin()+pointerOnSelectedVertex);
  pointerOnSelectedVertex=getPreviousSubscript();
  endPoints[pointerOnSelectedVertex].isSelected=true;
}


void specialKeyboard(int key, int x, int y)
{
  if (endPoints.empty())
    return;

  unsigned long int previousSubscript=getPreviousSubscript();
  switch (key)
    {
    case 111: //del
      {
	if((mode==VERTEX_MODE))
	  {
	    deletePointedVertex();
	  }
	else if (mode==EDGE_MODE)
	  {
	    if (endPoints.size()>1)
	      {
		deletePointedVertex();
		deletePointedVertex();
	      }
	  }
      }
      break;
    case GLUT_KEY_PAGE_UP:
      {
	if((mode==VERTEX_MODE)||(mode==EDGE_MODE))
	  {
	    // current selection set to false
	    endPoints[pointerOnSelectedVertex].isSelected=false;
	    // compute new subscript
	    pointerOnSelectedVertex++;
	    pointerOnSelectedVertex%=endPoints.size();
	    // new selected point is true
	    endPoints[pointerOnSelectedVertex].isSelected=true;

	  }
      }
      break;
    case GLUT_KEY_PAGE_DOWN:
      {
	if((mode==VERTEX_MODE)||(mode==EDGE_MODE))
	  {
	    // current selection set to false
	    endPoints[pointerOnSelectedVertex].isSelected=false;
	    pointerOnSelectedVertex=previousSubscript;
	    // new selected point is true
	    endPoints[pointerOnSelectedVertex].isSelected=true;
	  }
      }
      break;
    case GLUT_KEY_DOWN:
      {
	if (mode==VERTEX_MODE)
	  endPoints[pointerOnSelectedVertex]+=Vec2(0,1);
	else if (mode==EDGE_MODE)
	  {
	    endPoints[pointerOnSelectedVertex]+=Vec2(0,1);
	    endPoints[previousSubscript]+=Vec2(0,1);
	  }
      }
      break;
    case GLUT_KEY_UP:
      {
	if (mode==VERTEX_MODE)
	  endPoints[pointerOnSelectedVertex]+=Vec2(0,-1);
	else if (mode==EDGE_MODE)
	  {
	    endPoints[pointerOnSelectedVertex]+=Vec2(0,-1);
	    endPoints[previousSubscript]+=Vec2(0,-1);
	  }
      }
      break;
    case GLUT_KEY_LEFT:
      {
	if (mode==VERTEX_MODE)
	  endPoints[pointerOnSelectedVertex]+=Vec2(-1,0);
	else if (mode==EDGE_MODE)
	  {
	    endPoints[pointerOnSelectedVertex]+=Vec2(-1,0);
	    endPoints[previousSubscript]+=Vec2(-1,0);
	  }
      }
      break;
    case GLUT_KEY_RIGHT:
      {
	if (mode==VERTEX_MODE)
	  endPoints[pointerOnSelectedVertex]+=Vec2(1,0);
	else if (mode==EDGE_MODE)
	  {
	    endPoints[pointerOnSelectedVertex]+=Vec2(1,0);
	    endPoints[previousSubscript]+=Vec2(1,0);
	  }
      }
      break;
    }
}
/**
 * Callback appelé lorsqu'un évènement clavier est détecté.
 */
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
    {
    case 'c':
      {
	closePolygon=!closePolygon;
	printf("closePolygon:%d\n",closePolygon);
      }
      break;
    case 's':
      {
	smooth2(endPoints,closePolygon);
      }
      break;
    case 'f':
      {
	fillPolygon=!fillPolygon;
	printf("fillPolygon:%d\n",fillPolygon);
      }
      break;
    case 'a':
      {
	pointerOnSelectedVertex=0;
	mode=APPEND_MODE;
	printf("append mode\n");
      }
      break;
    case 'v':
      {
	mode=VERTEX_MODE;
	// reset the mode
	for (unsigned int i=0;i<endPoints.size();i++)
	  {
	    endPoints[i].isSelected=false;
	    endPoints[i].isSelectable=false;
	  }
	cropPointerOnSelectedVertex();
	endPoints[pointerOnSelectedVertex].isSelected=true;
	printf("vertex mode\n");
      }
      break;
    case 'e':
      {
	mode=EDGE_MODE;
	for (unsigned int i=0;i<endPoints.size();i++)
	  {
	    endPoints[i].isSelected=false;
	    endPoints[i].isSelectable=false;
	  }
	cropPointerOnSelectedVertex();
	endPoints[pointerOnSelectedVertex].isSelected=true;
	printf("edge mode\n");
      }
      break;
    case 'm':
      {
	if (mode==EDGE_MODE)
	  {
	    endPoints[pointerOnSelectedVertex].isSelected=false;
	    unsigned long int previousSubscript=getPreviousSubscript();
	    EndPoint newEndpoint=computeMiddle(endPoints[pointerOnSelectedVertex],endPoints[previousSubscript]);
	    endPoints.insert(endPoints.begin()+pointerOnSelectedVertex,newEndpoint);
	    endPoints[pointerOnSelectedVertex].isSelected=true;
	  }
      }
      break;
    case 27:             // ESCAPE key
      {
	exit (0);
      }
      break;

    }
}


//////////
// Main //
//////////



int main(int argc, char* argv[])
{
  // Initialize GLUT
  glutInit(&argc, argv);

  // GLUT parameters
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  // glutInitContextVersion(3, 3);
  // glutInitContextFlags(GLUT_CORE_PROFILE);	
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(0, 0);

  // Create the window before defining functions
  glutCreateWindow("OpenGL: 2D modeling software");

  // Define the callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyboard);

  glutMouseFunc(mouseClicked);
  glutMotionFunc(mouseMove);
  glutIdleFunc(idle);

  // Initialize GLEW
  glewExperimental = GL_TRUE; 

  GLenum err = glewInit();

  if(GLEW_OK != err)
    {
      std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

  initialiseGLState();
  initScene();


  // Enter the rendering main loop
  glutMainLoop();

  return 0;
}
