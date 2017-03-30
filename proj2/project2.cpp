/*
Name: Sri Satya Sai Kamal Atluri
Student ID: 109479597
Class: CSCI 272 - Computer Graphics
Project Name: Project2.cpp
*/
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
//#include <GL/freeglut.h>

using namespace std;
static int slices = 24; //This is used to draw the sphere
static int stacks = 24;
double xs = -3.5; //These are the coordinates for the sphere x,y,z
double ys= 0.0;
double zs = -1.0;
double xc = 4.0; //These are the coordinates for the cube x,y,z
double yc = 0.0;
double zc = -3.0;
double xt = 0.0 ; ////These are the coordinates for the teapot x,y,z
double yt =0.0;
double zt = 2.0;
double radius = 1.3; // The radius of the sphere
int sflag =0,cflag =0,tflag =0; // The flags to know which keys are pressed
bool keys[256];
double as=0.0; // These are the angles of rotation.
double ac=0.0;
double az=0.0;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
     {
         glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
         Ratio =  height/width;
      }
    else
      {
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
        Ratio = width /height;
      }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,Ratio,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLdouble size = 0.5;

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

	glPushMatrix();                     //Matrix to draw the cube.
        glTranslated(xc,yc,zc);
        glRotated(ac,0,1,0);
        glutSolidCube(3.0);
        glColor3f(0,0,1);
    glPopMatrix();

        glPushMatrix();       //Matrix to draw the sphere.
        glTranslated(xs,ys,zs);
         glRotated(as,0,1,0);
        glutSolidSphere(radius,slices,stacks);
        glColor3f(0,1,0);
    glPopMatrix();

    glPushMatrix ();    //Matrix to draw the Teapot.
        glTranslated(xt,yt,zt);
         glRotated(az,0,1,0);
        glutSolidTeapot ( 1.5 );
        glColor3f(1,0,0);
    glPopMatrix ();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27 :

        case 'q':
            exit(0);
            break;

        case 'c' : // When 'c' is pressed, all the remaining flags are set to 0 except cflag
        cflag=1;
        sflag =0;
        tflag =0;
    break;

      case 's' : // When 's' is pressed, all the remaining flags are set to 0 except sflag
      sflag =1;
      tflag =0;
      cflag =0;
    break;
      case 't': // When 't' is pressed, all the remaining flags are set to 0 except tflag
        tflag=1;
        sflag =0;
        cflag =0;
    break;

      case 'w':
        WireFrame =!WireFrame;
        break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP: // When 'Up_Key' is pressed, the translation is done based upon the key pressed before
        if(cflag==1)
         zc=zc+1;
        if(sflag==1)
         zs=zs+1;
        if(tflag==1)
         zt=zt+1;

    break;
    case GLUT_KEY_DOWN:  // When 'Down_Key' is pressed, the translation is done based upon the key pressed before
         if(cflag==1)
         zc=zc-1;
        if(sflag==1)
         zs=zs-1;
        if(tflag==1)
         zt=zt-1;
     break;
      case GLUT_KEY_LEFT: // When 'Left_Key' is pressed, the rotation is done based upon the key pressed before
         if(cflag==1){
         ac = ac+5;
         }
        if(sflag==1){
         as = as+5;
        }
        if(tflag==1){
        az=az+5;
        }
     break;
      case GLUT_KEY_RIGHT:  // When 'Right_Key' is pressed, the rotation is done based upon the key pressed before
         if(cflag==1){
        ac = ac-5;
         }
        if(sflag==1){
              as = as-5;
        }
        if(tflag==1){
         az=az-5;
        }
     break;
   }
;
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
