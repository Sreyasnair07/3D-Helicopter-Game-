#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const GLfloat ViewAngleOffset=0.01;
const GLfloat ZoomRadiusOffset=0.05; //front back

const GLfloat SIZE1= 1000;			// To deal with the view volume
const GLfloat NEAR_Z= 0.001;
const GLfloat FAR_Z= 100.0;
//constant PI
const GLfloat PI= 3.14159265;		// An excessively abused used constant !!
//utility function for drawing models
void drawOval(GLfloat x, GLfloat y, GLfloat z);
void drawSphere(float fRadius, GLint slices, GLint rings);
void drawCylinder(float fTopR, float fBottomR, float fHeight);
//small helper functions
GLfloat deg2rad(GLfloat degree);
//model helper functions for each module
void drawHelicopter();
void drawScene();
//setup functions
void init (void);
void showMenu(void);
//callback functions
void keyboardCallbackProc(unsigned char key, int x, int y);
//void specialCallbackProc (int key, int x, int y);
const GLfloat ViewStartRadius=5;
const GLfloat ViewStartTheta=60;
const GLfloat ViewStartPhi=90;
int touchx=0;
int lev=1;
int m=0;
int rings[][3]={{-50,0,0},{-100,4,4},{-150,2,-4},{-200,6,6},{-250,4,-6}};
int kount=0;
int start=1;
const GLfloat MovementSpeed=0.3;

GLfloat mainRotorSpinSpeed=0;
GLfloat mainRotorRotatingAngle=0;
bool bCamera=false;
GLfloat viewPortw, viewPorth;

bool bWire;
GLfloat fovy = 90.0;		// For Perspective projections.
int H= SIZE1, W= SIZE1;

enum {PERSPECTIVE} ePrjn = PERSPECTIVE;
GLfloat move_x=0, move_z=0,move_y=0;
//the polar coordinate parameter
GLfloat theta=deg2rad(ViewStartTheta), phi=deg2rad(ViewStartPhi);

GLfloat radius=7;
//ViewStartRadius;
GLfloat deg2rad(GLfloat degree)
{
	return degree/360.0*2*PI;
}
void showMenu(void)				// Self Explanatory.
{
	printf("\n\n\n\n3D HELICOPTER GAME");
	printf("\n---------------------------------------------------------------");

	printf("\n Use m/M to see the menu again.");
	printf("\n Use escape key to exit.\n");
    printf("\nSCORE=%d",touchx);
    printf("\nMOVE_x=%f",move_x);
    printf("\nMOVE_y=%f",move_y);
    printf("\nMOVE_z=%f",move_z);
    printf("\n m=%d",m);
    printf("\nLEVEL=%d",lev);

}
void init (void)						// Initializes the gl Graphics env and the program variables.
{
	glMatrixMode(GL_PROJECTION);							// Set the current OpenGL matrix to GL_PROJECTION ie projection matrix.
	glLoadIdentity();						// Load identitiy values in the above.
	if(ePrjn == PERSPECTIVE)
	{
		gluPerspective(fovy, (GLfloat)W/(GLfloat)H, NEAR_Z, FAR_Z);
							// This sets the view volume to be a Perspective one.
	}
	glMatrixMode(GL_MODELVIEW);// Change the current matrix mode to Model-View matrix.
	glLoadIdentity();

	glClearColor (0.0, 0.8, 1, 0.3);						// set the background color to black.
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	bWire = false;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	showMenu();
}

void keyboardCallbackProc(unsigned char key, int x, int y)						// This is the callback procedure for capturing OpenGL Keyboard events.
{
	GLfloat temp;
	switch(key)
	{
	case ' ':
		bCamera=true;
		break;

	case 'L':
	case 'l':
		move_z-=MovementSpeed;
		break;
	case 'J':
    case 'j':
		move_z+=MovementSpeed;
		break;
	case 'K':
    case 'k':
        move_y-=MovementSpeed;
        break;

     case 'I':
     case 'i':
        move_y+=MovementSpeed;

        break;
	case 27 :				//ESCAPE Code for exiting program.
		exit(1);
		break;
	}
	glutPostRedisplay();	// Direct the glut system to redisplay the screen.
}

void reShapeCallbackProc(int w, int h)						// This is the callback procedure for capturing reShape event for window resizing.
{
	glViewport(0, 0, w, h);// Set the viewport to current window size.
	W = w;
	H = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(ePrjn == PERSPECTIVE)
	{
		gluPerspective(fovy, (GLfloat)W/(GLfloat)H, NEAR_Z, FAR_Z);
	}
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void drawCylinder(float fTopR, float fBottomR, float fHeight)
{
	GLUquadricObj* pObj;
							// To keep the original texture intact we need to set the current color to WHITE
	pObj = gluNewQuadric();
							// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluCylinder(pObj, fTopR, fBottomR, fHeight, 20, 20);
							// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);
							// Free the Quadric
}

void renderBitmapString(float x,float y,float z,void *font,char*string)
{
char *c;
glRasterPos3f(x, y,z);
for(c=string; *c != '\0'; c++)
{
glutBitmapCharacter(font, *c);
}
}

void drawScene()
{
	int i,k;
	const GLfloat GroundHeight=-10;
	const GLfloat View_Size=350;
	glPushMatrix();
		glColor4f(0.0, 0.8, 0.0,1);
		glLineWidth(2);

			glBegin(GL_POLYGON);
			glVertex3f(-View_Size, GroundHeight,
				-View_Size);
			glVertex3f(View_Size, GroundHeight,
				-View_Size);

			glVertex3f(View_Size, GroundHeight, View_Size);
			glVertex3f(-View_Size, GroundHeight, View_Size);
			glEnd();

	glColor4f(1,0,0.7,1);
		glPushMatrix();
			glTranslatef(-20, 2, -6);
				glPushMatrix();
                    glTranslated(0,0,7);
                    renderBitmapString(0, -0.5/1.3,0,GLUT_BITMAP_HELVETICA_18,"LEVEL  ");
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (lev+48));
                glPopMatrix();
		glPopMatrix();
	glPopMatrix();

for(k=0;k<5;k++)
{
glPushMatrix();
glTranslated(rings[k][0],rings[k][1],rings[k][2]);
	glColor4f(lev*0.2,0.9-k*0.2,0.1+k*0.2,1);
	glRotatef(90,0,1,0);
    glScaled(3,3,3);
	glutSolidTorus(0.2,2, 30, 30);
glPopMatrix();
}
}
void drawOval(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();

	glScalef(x, y, z);
	glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
}
void drawBlade()
{
	const GLfloat BladeWidth=0.1;
	const GLfloat BladeHeight=0.08;
	const GLfloat BladeLength=1.0;//always 1.0

	glPushMatrix();
		glTranslatef(0, 0, BladeLength);
        drawOval(BladeWidth, BladeHeight, BladeLength);
	glPopMatrix();
}
void drawMyRotor()
{
  glPushMatrix();
 glPushMatrix();
glColor4f(1,1,1,0.11);
 glTranslated(0,.4,0);
 drawBlade();
 glRotated(120,0,1,0);
 drawBlade();
 glRotated(120,0,1,0);
 drawBlade();
 glPopMatrix();
glTranslated(0,.4,0);
 glRotated(90,1,0,0);
glColor4f(0,0.5,1,1);
 drawCylinder(.08,.08,.20);
glPopMatrix();
}
void helibody()
{
 glPushMatrix();
 glTranslated(1.4,0,0);
 glColor4f(1,.7,0,.1);
 drawOval(.08,.08,.08);
 glPopMatrix();

 glPushMatrix();
 glTranslated(.5,0,0);
 glRotated(90,0,1,0);
 glScaled(.6,.8,.8);
 glColor4f(0,0.5,1,1);
 drawCylinder(.2,.1,1.0);
 glPopMatrix();

 glPushMatrix();
 glColor4f(1,.7,0,1);
 drawOval(0.7,0.4,0.4);
 glPopMatrix();

 glPushMatrix();
 glTranslated(-.31,.068,0);
 glRotated(-5,0,0,1);
 glColor4f(0,1,1,.3);
 drawOval(0.33,0.3,0.3);
 glPopMatrix();


 glPushMatrix();
 glTranslated(0,.3,0);
 glScaled(.5,.5,.5);

 glRotatef(mainRotorRotatingAngle, 0,1,0);
 drawMyRotor();
 glPopMatrix();
}

void drawHelicopter()
{
	glPushMatrix();
    glScaled(2,2,2);
	helibody();
	glPopMatrix();
}

void displayCallbackProc (void)
{
	GLfloat eye_x, eye_y, eye_z;
	const GLfloat X_Offset=1.5;
	const GLfloat Y_Offset=-1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
    mainRotorSpinSpeed=15;
	mainRotorRotatingAngle+=mainRotorSpinSpeed;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
/************************************************************
The view transformation
*************************************************************/
	eye_x=radius*sin(theta)*sin(phi);
	eye_y=radius*cos(theta);
	eye_z=radius*sin(theta)*cos(phi);
	if (bCamera)
	{
		gluLookAt(eye_x+move_x, eye_y, eye_z+move_z, move_x, 0, move_z, 0, 1, 0);
  renderBitmapString(move_x,0+4,move_z-4,GLUT_BITMAP_HELVETICA_18,"SCORE : ");
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ((touchx)+48));
  renderBitmapString(move_x,0+3.8,move_z-4,GLUT_BITMAP_HELVETICA_18," LEVEL : ");
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ((lev/10)+48));
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ((lev%10)+48));

		glTranslatef(-eye_x, -eye_y, -eye_z);
		drawScene();
		//translate helicopter a little back so that pilot is at rotation axis
		glTranslatef(X_Offset+move_x, Y_Offset+move_y,move_z);
    move_x-=MovementSpeed*0.1*lev;
        if(move_x<=(1.5+rings[m][0])&&move_x>=(rings[m][0]+1.30))
        if(move_y<=(3.60+rings[m][1])&&move_y>=(rings[m][1]-3.60))
        if(move_z<=(3.80+rings[m][2])&&move_z>=(rings[m][2]-3.80))
        if(++kount==1)
        touchx++;
        if(move_x<=rings[m][0]&&move_x>=(rings[m][0]-0.2))
        {m++;
        kount=0;
        }

        if(move_x<=-280)
        {

        if(touchx>=3)
        {
        lev++;
        }
        move_x=30;
        m=0;
        touchx=0;
        }
        glScalef(2, 2, 2);

		drawHelicopter();
	}


	else{

		gluLookAt(eye_x+move_x, eye_y, eye_z+move_z+8, move_x, 0, move_z, 0, 1, 0);

    glScaled(1,1,1);
  renderBitmapString(move_x,5,move_z+3,GLUT_BITMAP_HELVETICA_18,"HELICOPTER GAME");
  renderBitmapString(move_x,3,move_z+4,GLUT_BITMAP_HELVETICA_18,"DEVELOPED BY:");

  renderBitmapString(move_x,2,move_z,GLUT_BITMAP_HELVETICA_18,"SREYAS P(4MH18CS107)");
  renderBitmapString(move_x,1,move_z,GLUT_BITMAP_HELVETICA_18,"SATHWIK N(4MH18CS098)");

renderBitmapString(move_x,-1,move_z+3,GLUT_BITMAP_HELVETICA_18,"SPACE TO CONTINUE");
renderBitmapString(move_x,-2,move_z+3,GLUT_BITMAP_HELVETICA_18,"PRESS I FOR UP");
renderBitmapString(move_x,-3,move_z+3,GLUT_BITMAP_HELVETICA_18,"PRESS K FOR DOWN");
renderBitmapString(move_x,-4,move_z+3,GLUT_BITMAP_HELVETICA_18,"PRESS J FOR LEFT");
renderBitmapString(move_x,-5,move_z+3,GLUT_BITMAP_HELVETICA_18,"PRESS L  FOR RIGHT");
renderBitmapString(move_x,-6,move_z+3,GLUT_BITMAP_HELVETICA_18,"PRESS ESC TO EXIT");

	}



	glutSwapBuffers();	// Use of double buffering to avoid flicker.
	glutPostRedisplay();
}

/****************************************************************************************/
int main (int argc, char *argv[])						// The main program.
{	/* All customary glut env initializations. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SIZE1, SIZE1);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Helicopter game");
	init();
	glutDisplayFunc(displayCallbackProc);
	glutKeyboardFunc(keyboardCallbackProc);
//	glutSpecialFunc(specialCallbackProc);
	glutReshapeFunc(reShapeCallbackProc);
	/* Callback registrations done.*/
	glutMainLoop();
	return 1;
}
