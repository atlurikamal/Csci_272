/*
Name: Sri Satya Sai Kamal Atluri
Student ID: 109479597
Subject: Computer Graphics
Class: CSCI-272
Program: project5.cpp
*/
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <math.h>
using namespace std;
double * vx, * vy, *vz, *vnz,*vnx,*vny, *fvx, *fvy,*fvz,*fvnx, *fvny,*fvnz; // These pointers are used to store the data which is read from the file.
string l, r; // Temporary variables
int j=0,k=0,m=0;
int nx,ny,nz,vbx,vby,vbz,i;
int vstart, vend, vnend;
double trans =-10;// Used for Zooming In/Out.
double ax=0,ay=0; // Used for Rotating Up/Down, Left/Right.
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
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    glTranslatef(0.0,0.0,trans); // This is used for translating forward and backward for Zooming In and Out.
    glRotated(ax,1,0,0); // This is used for rotating Up/Down with respect to X-axis.
    glRotated(ay,0,1,0); // This is used for rotating Left/Right with respect to Y-axis.
    glPushMatrix(); // Pushing the matrix.
    //glPointSize(2.0);

    for(i =0;i<22250;i++){// This loop is runned for 22250 times because it has 22250 faces.

    glBegin(GL_TRIANGLES);
    nx=fvnx[i]; // The values are loaded into the variables.
    ny=fvny[i];
    nz=fvnz[i];
    vbx=fvx[i];
    vby=fvy[i];
    vbz=fvz[i];


    //cout<<vx[vbx]<<"||"<<vy[vbx]<<"||"<<vz[vbx]<<endl;
    glNormal3d(vnx[nx],vny[nx],vnz[nx]);//This is to get the normals for Point A in the triangles.
    glVertex3d(vx[vbx],vy[vbx],vz[vbx]);//This is to plot the point A for the triangles.
    //cout<<vx[vby]<<"||"<<vy[vby]<<"||"<<vz[vby]<<endl;
    glNormal3d(vnx[ny],vny[ny],vnz[ny]);//This is to get the normals for Point B in the triangles.
    glVertex3d(vx[vby],vy[vby],vz[vby]);//This is to plot the point B for the triangles.
    //cout<<vx[vbz]<<"||"<<vy[vbz]<<"||"<<vz[vbz]<<endl;
    glNormal3d(vnx[nz],vny[nz],vnz[nz]);//This is to get the normals for Point C in the triangles.
    glVertex3d(vx[vbz],vy[vbz],vz[vbz]);//This is to plot the point C for the triangles.



    glEnd();
 }
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':// When the q is pressed, the space is set to free because to avoid the data memory wastage.
            free(vx);
            free(vy);
            free(vz);
            free(vnx);
            free(vny);
            free(vnz);
            free(fvx);
            free(fvy);
            free(fvz);
            free(fvnx);
            free(fvny);
            free(fvnz);
            exit(0);
            break;
        case 'w': // This is get the Wireframe when w is pressed.
            WireFrame =! WireFrame;
            break;
    }
      glutPostRedisplay();
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_END://This is to Zoom In
        trans+=1;
    break;

    case GLUT_KEY_HOME:// This is to Zoom out.
        trans-=1;
    break;
        case GLUT_KEY_UP://This is for Rotate UP X-axis
        ax+=1;
    break;

    case GLUT_KEY_DOWN://This is for Rotate DOWN X-axis
        ax-=1;
    break;
    case GLUT_KEY_LEFT://This is for rotate left
        ay+=1;
    break;

    case GLUT_KEY_RIGHT://This is for rotate right
        ay-=1;
    break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)// Here the Initialization process goes on, the file is read into the variales.
{

        string line,v="v",vn="vn",f="f",temp,d="d";
        int factor = 500;
    int pos,fflag=0,vflag=0,vnflag=0,vtflag=0,vnstart=0,fstart = 0 ;
  ifstream myfile ("ateneam.obj"); //Giving path to the object file and defining the file as myfile.
  if (myfile.is_open()) // Checking whether the file is open or not.
  {

    while ( getline (myfile,line) ) // Getting line by line to process further.
    {

        pos = line.find(" "); // Finding the position for the space.
        l= line.substr(0,pos); // Storing the left part into the string.
        r= line.substr(pos+1); // Storing the right part into the string.

    if(l==d){ //If d line is read, then we will dynamically allocate the space for the variables.
            int num;
           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek(l);//X //Converting the string into double.
    	   geek >> num;

            vx = (double*) malloc(8*num);
            vy = (double*) malloc(8*num);
            vz = (double*) malloc(8*num);

           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek1(l); //Converting the string into double.
    	   geek1 >> num;
            vnz= (double*) malloc(8*num);
            vnx= (double*) malloc(8*num);
            vny= (double*) malloc(8*num);
           stringstream geek2(r); //Converting the string into double.
    	   geek2 >> num;
            fvx=(double*) malloc(8*num);
            fvy=(double*) malloc(8*num);
            fvz=(double*) malloc(8*num);
            fvnx=(double*) malloc(8*num);
            fvny=(double*) malloc(8*num);
            fvnz=(double*) malloc(8*num);
       }

    if(vnflag == 0)//The vnflag is set to 1 when vertices scanning is done.
    {
             if(l==v)
             {
        vstart =j;


           pos=r.find(" "); //S // Finding the position for the space.
           l=r.substr(0,pos);//S // Storing the left part into the string.
           r=r.substr(pos+1);//SXSYSZ // Storing the right part into the string.


           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek(l);//X //Converting the string into double.
    	   geek >> vx[j];
    	   //cout<<vx[j]<<" ";
    	   vx[j] = vx[j]/factor;


    	   pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos); // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           stringstream geek1(l); //Converting the string into double.
    	   geek1 >> vy[j];
    	   //cout<<vy[j]<<" ";
    	   vy[j] = vy[j]/factor;


    	   pos=r.find(" "); // Finding the position for the space.
           l=r.substr(0,pos); // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           stringstream geek2(l);  //Converting the string into double.
    	   geek2 >> vz[j];
    	   //cout<<vz[j]<<endl;
    	   vz[j] = vz[j]/factor;

    	   j++;
    	   }
    	   else if(vstart >=11145){
            vnflag = 1;
    	   }
      }else if(fflag == 0){// The fflag is set to 1 when we done with reading the vn values.

        pos = line.find(" "); // Finding the position for the space.
        l= line.substr(0,pos); // Storing the left part into the string.
        r= line.substr(pos+1); // Storing the right part into the string.

        if(l==vn){

                vnstart=k;

           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYS // Storing the right part into the string.

           stringstream geek(l);//X //Converting the string into double.
    	   geek >> vnx[k];


    	   pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos); // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           stringstream geek1(l); //Converting the string into double.
    	   geek1 >> vny[k];


    	   pos=r.find(" "); // Finding the position for the space.
           l=r.substr(0,pos); // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           stringstream geek2(l); //Converting the string into double.
    	   geek2 >> vnz[k];


k++;
      }else if(vnstart != 0){
      fflag=1;
      }


      }else if(vtflag == 0){


        pos = line.find(" "); // Finding the position for the space.
        l= line.substr(0,pos); // Storing the left part into the string.
        r= line.substr(pos+1); // Storing the right part into the string.



           if(l==f){

           fstart = m;

           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           temp = r;

           r = l;


           pos=r.find("/");//S // Finding the position for the '/'
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek(l);//X //Converting the string into double.
    	   geek >> fvx[m];

    	   fvx[m]--;

           pos=r.find("/");//S // Finding the position for the '/'.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.


    	   stringstream geek2(r); //Converting the string into double.
    	   geek2 >> fvnx[m];

    	   fvnx[m]--;

    	   r = temp;


           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           temp = r;

           r = l;

           pos=r.find("/");//S // Finding the position for the '/'.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek1(l);//X //Converting the string into double.
    	   geek1 >> fvy[m];

    	   fvy[m]--;

           pos=r.find("/");//S // Finding the position for the '/'.
           l=r.substr(0,pos);//X  // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.


    	   stringstream geek3(r); //Converting the string into double.
    	   geek3 >> fvny[m];

    	   fvny[m]--;


           r = temp;

           pos=r.find(" ");//S // Finding the position for the space.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1); // Storing the right part into the string.

           temp = r;

           r = l;

           pos=r.find("/");//S // Finding the position for the '/'.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.

           stringstream geek4(l);//X //Converting the string into double.
    	   geek4 >> fvz[m];

    	   fvz[m]--;

           pos=r.find("/");//S // Finding the position for the '/'.
           l=r.substr(0,pos);//X // Storing the left part into the string.
           r=r.substr(pos+1);//SYSZ // Storing the right part into the string.


    	   stringstream geek5(r); //Converting the string into double.
    	   geek5 >> fvnz[m];

    	   fvnz[m]--;


      m++;  } else if( fstart !=0){
        vtflag =1;
        }

      }
    }
    myfile.close();



  }

  else cout << "Unable to open file";

  //for(i=11140;i<=11147;i++)
  //cout<<vx[i]<<" "<<vy[i]<<" "<<vz[i]<<endl;
  //cout<<vx[2]<<" "<<vy[2]<<" "<<vz[2]<<endl;
  //cout<<vx[3]<<" "<<vy[3]<<" "<<vz[3]<<endl;
  //cout<<vx[4]<<" "<<vy[4]<<" "<<vz[4]<<endl;
  //cout<<vx[7]<<" "<<vy[7]<<" "<<vz[7]<<endl;
  //cout<<vx[8]<<" "<<vy[8]<<" "<<vz[8]<<endl;
  //cout<<vx[9]<<" "<<vy[9]<<" "<<vz[9]<<endl;
  //cout<<vx[10]<<" "<<vy[10]<<" "<<vz[10]<<endl;


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

            free(vx); // The space is set to free because to avoid the data memory wastage.
            free(vy);
            free(vz);
            free(vnx);
            free(vny);
            free(vnz);
            free(fvx);
            free(fvy);
            free(fvz);
            free(fvnx);
            free(fvny);
            free(fvnz);

    return EXIT_SUCCESS;
}
