/////////////////////////////////////////////////////////////////////         
// Tyler Goodwyn's assignment 2
//
// This program is a game. It begins by drawing a sphere and a cube to the screen, 
// using an orthographic projection and double buffering.  The goal of the game is to move the sphere inside
// of the cube using the aswd keys.

// Included functions are:
// writeBitmapString that takes a string and renders it to the screen in a bitmap font
// displayWin() that writes text to the screen when the user has completed the goal
// setup() that initializes the cube to a random location, 
//   ensuring that location is within the screen and not overlapping the sphere
// animate() that does the actual translation and is called once per frame, for 5 frames, every 20ms
// specialKeyInput() that allows the user to use the alternate inputs of the arrow keys
///////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <time.h> // the time library is included for seeding the rand function

#define PI 3.14159265358979324
#define NUMFRAMES 5 // the number of frames per animation
#define TRANSLATION 10 // the total translation per animation
#define TRANSPERFRAME 2 // equal to TRANSLATION / NUMFRAMES
#define ANIMATIONPERIOD 20 // the amount of ms the frames will be spaced apart
#define CUBEWIDTH 50 // dimension of the cube

// Globals.
static float Xvalue = 0.0, Yvalue = 0.0; // Co-ordinates of the sphere.
static int frameCount = 1; // a variable to track the number of frames displayed by the current animation
static float cubeStartX = 0.0, cubeStartY = 0.0; // starting co-ordinates of the cube
static long font = (long)GLUT_BITMAP_9_BY_15; // Font selection.

// Function for writing text to the screen
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// function called when the user has completed the objective of the game
void displayWin(void)
{
	glColor3f(0.0, 0.0, 0.0);
	// the identity matrix is loaded so that the text can be easily set at an absolute position on the screen
	// before doing this a matrix is pushed onto the stack so that other object's rendering is not affected
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3f(220.0, 250.0, 0.0);
	char point[10];
	//strcpy_s(point, "YOU WON!\0");
	writeBitmapString((void*)font, "YOU WON!");
	glPopMatrix();
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	// the world origin is translated to the center of the window
	glTranslatef(250.0, 250.0, 0.0);

	// the sphere's rendering
	glPushMatrix();
	glColor3f(255.0, 0.0, 0.0);
	// variable coordinates based on user input + animation
	glTranslatef(Xvalue, Yvalue, 0.0);
	glutWireSphere(15, 10, 10);
	glPopMatrix();

	// the cube's rendering
	glColor3f(0.0, 255.0, 0.0);
	glTranslatef(cubeStartX, cubeStartY, 0.0);
	// a check for the victory condition
	if (
		Xvalue < (cubeStartX + CUBEWIDTH/2) && 
		Xvalue > (cubeStartX - CUBEWIDTH/2) &&
		Yvalue < (cubeStartY + CUBEWIDTH/2) &&
		Yvalue > (cubeStartY - CUBEWIDTH/2)
		)

	{
		displayWin();
		// the color will also changed when goal has been reached
		glColor3f(0.0, 0.0, 255.0);
	}
	glutWireCube(CUBEWIDTH);
	
	// necessary for double buffering
	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// rand is seeded with the current time
	srand(time(NULL));

	// the cube's initial position is set to be random
	do 
	{
		cubeStartX = rand() % 450 - 225;
		cubeStartY = rand() % 450 - 225;
	// this position cannot overlap with that of the sphere's
	// so it will be redetermined if it does
	} while (
		Xvalue < (cubeStartX + CUBEWIDTH / 2) &&
		Xvalue >(cubeStartX - CUBEWIDTH / 2) &&
		Yvalue < (cubeStartY + CUBEWIDTH / 2) &&
		Yvalue >(cubeStartY - CUBEWIDTH / 2)
		);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// the animate function that gets called when a user presses a key
void animate(int direction)
{
	// key presses are mapped to different translations
	switch (direction)
	{
	case 1:
		Xvalue -= TRANSPERFRAME;
		break;
	case 2:
		Yvalue += TRANSPERFRAME;
		break;
	case 3:
		Xvalue += TRANSPERFRAME;
		break;
	case 4:
		Yvalue -= TRANSPERFRAME;
		break;
	}
	glutPostRedisplay();

	// the animation lasts 5 frames
	frameCount++;
	if (frameCount >= NUMFRAMES)
	{
		frameCount = 1;
		return;
	}
	// the period is 20 ms
	glutTimerFunc(ANIMATIONPERIOD, animate, direction);


}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	// different key presses call the animation function with different parameters
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'a':
		animate(1);
		break;
	case 'w':
		animate(2);
		break;
	case 'd':
		animate(3);
		break;
	case 's':
		animate(4);
		break;
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	// alternate keys for calling the animation function
	if (key == GLUT_KEY_UP) animate(2);
	if (key == GLUT_KEY_DOWN) animate(4);
	if (key == GLUT_KEY_LEFT) animate(1);
	if (key == GLUT_KEY_RIGHT) animate(3);
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press adws to move the sphere left/right/up/down." << std::endl;
	std::cout << "Move the sphere into the cube to beat the game." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	// uses the double buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 2");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	// Register the callback function for non-ASCII key entry.
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
