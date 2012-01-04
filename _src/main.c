// Computer Graphics Project
//Shaswot Singh Adhikari 8333
//Ashish Kayastha 8308

#include <stdlib.h>
#include <GL/glut.h> // Header file for Graphics Library Utility Toolkit (GLUT)

// Constants defined for use in the program
#define FLAT_MODE 0
#define SMOOTH_MODE 1
#define WIREFRAME 2
#define LIGHTING 3
#define TEAPOT 4
#define TORUS 5
#define CONE 6
#define SPHERE 7
#define EXIT 8

// Default values for the scene
static int mode = SMOOTH_MODE;
static int wireframe = 0;
static int lighting = 1;
static int xspin = 0, yspin = 0;
static int xbegin, ybegin;
static int object = TEAPOT;
static int slices = 16;
static int stacks = 16;

// Rotation amounts
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

// Light values and coordinates
GLfloat	 lightPos[] = { 0.0f, 0.0f, 75.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f};
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };

void ProcessMenu(int value)
{
	switch(value) {
		case 0:
			mode = FLAT_MODE;
			break;
		case 1:
			mode = SMOOTH_MODE;
			break;
		case 2:
			wireframe = !wireframe;
			break;
		case 3:
			lighting = !lighting;
			break;
		case 4:
			object = TEAPOT;
			break;
		case 5:
			object = TORUS;
			break;
		case 6:
			object = CONE;
			break;
		case 7:
			object = SPHERE;
			break;
		case 8:
			exit(0);
	}
	// Refresh the window
	glutPostRedisplay();
}

// Function to draw the scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(mode == FLAT_MODE)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);

	if(lighting)
		// Turn on lights
		glEnable(GL_LIGHTING);
	else
		// Turn off lights
		glDisable(GL_LIGHTING);

    // First place the light 
    // Save the coordinate transformation
    glPushMatrix();	
        // Rotate coordinate system
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);

        // Specify new position and direction in rotated coordinates
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

        // Set the color to red to draw a red cone
		glColor3ub(255, 0, 0);
		// Translate origin to move the cone out to where the light is positioned
		glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
		
		if(!lighting) {
			// Save the lighting state variables
			glPushAttrib(GL_LIGHTING_BIT);
				glEnable(GL_LIGHTING);
				// Draw a red cone to enclose the light source
				glutSolidCone(4.0f, 6.0f, 15, 15);
			glPopAttrib();
		}
		else {
			// Draw a red cone to enclose the light source
			glutSolidCone(4.0f, 6.0f, 15, 15);
		}
        // Draw a smaller displaced sphere to denote the light bulb
        // Save the lighting state variables
        glPushAttrib(GL_LIGHTING_BIT);

            // Turn off lighting and specify a bright yellow sphere
            glDisable(GL_LIGHTING);
            if(lighting)
				glColor3ub(255, 255, 0);
			else
				glColor3f(0.75f, 0.75f, 0.75f);
            glutSolidSphere(3.0f, 15, 15);

        // Restore lighting state variables
        glPopAttrib();

	// Restore coordinate transformations
	glPopMatrix();
	
	// Set material color
	if(lighting)
		glColor3ub(255, 255, 255);
	else
		glColor3f(0.25f, 0.25f, 0.25f);

	glPushMatrix();	
    // Rotate coordinate system
    glRotatef(xspin, 0.0f, 1.0f, 0.0f);
	glRotatef(yspin, 1.0f, 0.0f, 0.0f);

	switch (object) {
		case TEAPOT:
			// Draw Teapot
			if(wireframe) glutWireTeapot(40.0f);
			else glutSolidTeapot(40.0f);
			break;
		case TORUS:
			// Draw Torus
			if(wireframe) glutWireTorus(10.0f, 40.0f, slices, stacks);
			else glutSolidTorus(10.0f, 40.0f, slices, stacks);
			break;
		case CONE:
			// Draw Cone
			if(wireframe) glutWireCone(40.0f, 70.0f, slices, stacks);
			else glutSolidCone(40.0f, 70.0f, slices, stacks);
			break;
		case SPHERE:
			// Draw Sphere
			if(wireframe) glutWireSphere(40.0f, slices, stacks);
			glutSolidSphere(40.0f, slices, stacks);
			break;
	}

	glPopMatrix();
	// Display the results
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering context
void init()
{
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	
	// Setup and enable light 0
	// Supply a slight ambient light so the objects can be seen
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	// The light is composed of just a diffuse and specular components
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Specific spot effects
	// Cut off angle is 50 degrees
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);

	// Enable this light in particular
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    xbegin = x;
	ybegin = y;
  }
}

void motion(int x, int y)
{
	xspin = (xspin + (x - xbegin)) % 360;
	xbegin = x;
	yspin = (yspin + (y - ybegin)) % 360;
	ybegin = y;

	// Refresh the window
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y)
{
    switch(key) {
		case GLUT_KEY_UP:
			xRot -= 5.0f;
			break;
		case GLUT_KEY_DOWN:
			xRot += 5.0f;
			break;
		case GLUT_KEY_LEFT:
			yRot -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			yRot += 5.0f;
			break;
	}

	// Refresh the Window
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
		case ']':
			slices++;
			stacks++;
			break;
		case '[':
			if(slices>3 && stacks>3) {
				slices--;
				stacks--;
			}
			break;
		case '1':
			object = TEAPOT;
			break;
		case '2':
			object = TORUS;
			break;
		case '3':
			object = CONE;
			break;
		case '4':
			object = SPHERE;
			break;
		case 's':
		case 'S':
			if(mode == SMOOTH_MODE)
				mode = FLAT_MODE;
			else
				mode = SMOOTH_MODE;
			break;
		case 'w':
		case 'W':
			wireframe = !wireframe;
			break;
		case 'l':
		case 'L':
			lighting = !lighting;
			break;
		case 27:
			exit(0);
	}

	// Refresh the Window
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	GLfloat aspect;
    
	// Prevent a divide by zero
	if(h == 0)
	h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish viewing volume
	aspect = (GLfloat) w / (GLfloat) h;
	gluPerspective(35.0f, aspect, 1.0f, 500.0f);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -250.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Lighting Effects with OpenGL");

	// Create the Menu
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Flat Shading", FLAT_MODE);
	glutAddMenuEntry("Smooth Shading", SMOOTH_MODE);
	glutAddMenuEntry("Toggle Wireframe [W]", WIREFRAME);
	glutAddMenuEntry("Toggle Lighting [L]", LIGHTING);
	glutAddMenuEntry("Teapot [1]", TEAPOT);
	glutAddMenuEntry("Torus [2]", TORUS);
	glutAddMenuEntry("Cone [3]", CONE);
	glutAddMenuEntry("Sphere [4]", SPHERE);
	glutAddMenuEntry("Exit [ESC]", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
    
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(RenderScene);
	init();
	glutMainLoop();

    return 0;
}