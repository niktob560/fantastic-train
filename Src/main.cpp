#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <signal.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <FTGL/ftgl.h>
#include <FTGL/FTGLPixmapFont.h>

#include "routines.hpp"
#include "findwayBases.hpp"

#define GL_TIMER_DELAY	40
#define RETURN_CODE_OK 0
#define OBST_COLOR 0xFF0000

#define VECTORS_PER_CHECK	10

size_t QUAD_CHECK_SIDE = 20;

using namespace std;


bool terminated = false;
size_t numOfObstacles = 4;


struct obstacle *obstacles;
struct coords	startway,
				endway;



struct array
{
	void *items;
	size_t size;
};







void calculateWay()
{

}



/*
 *	-------+----->	0
 *	<------+------	180
 *		  /|\
 *         |		90
 *		   +
 */
struct vect createVect(const struct obstacle *obst, const uint8_t corner, const COORDS_DATATYPE angle)
{
	struct vect ret;
	ret.c  = (getCoordsOfCorner(obst, corner));
	ret.dx = QUAD_CHECK_SIDE;
	ret.dy = ret.dx * tan(angle);
	return ret;
}

struct array* getDataSet(const struct obstacle *obst, const uint8_t corner)
{
	struct graphPoint *p = getPoint(obst, corner);
	size_t size = 0;
	while (size == 0) {
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			struct coords *c = getCoordsOfCorner(obst, corner);
			if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE / 2)
				size++;
		}
		if(size == 0)
			QUAD_CHECK_SIDE += 10;
	}
	size_t iter = 0;
	struct obstacle **obsts = (struct obstacle**)malloc(sizeof(struct obstacle*) * size);
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		struct coords *c = getCoordsOfCorner(obst, corner);
		if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE / 2)
			obsts[iter++] = &obstacles[i];
	}
	struct array *ret = (struct array*)malloc(sizeof(struct array));
	ret->items = obsts;
	ret->size = size;
	return ret;
}


bool hasIntersections(const struct vect *v)
{
	for(size_t i = 0; i < numOfObstacles; i++)
	{

	}
}



void initObst(const struct obstacle *obst)
{
	struct graphPoint *point = getPoint(obst, CORNER_LEFT_BOT);
	struct array *obsts = getDataSet(obst, CORNER_LEFT_BOT);
	for(uint8_t i = 0; i < VECTORS_PER_CHECK; i++)
	{
		createVect(obst, CORNER_LEFT_BOT, ((360 / VECTORS_PER_CHECK) * i));
	}
}










void drawObstacle(const struct obstacle *o)
{
	glSetColor(OBST_COLOR);
	drawQuad(o->c->x, o->c->y, o->a);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

		// glSetColor(0xFF0000);
		// drawTriangle(glutGet(GLUT_WINDOW_WIDTH) / 2 - 200, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 200, 400);
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			drawObstacle(&obstacles[i]);
		}
		glSetColor(0x00FF00);
		drawLine(startway.x, startway.y, endway.x, endway.y);


    glutSwapBuffers();
}


void handleKeypress(unsigned char key, //The key that was pressed
                                        int x, int y) //The current mouse coordinates
{
    cout << "PRESSED\n" << (int)key << endl;
    if(key == 27) //ESC
    {
	    cout << "ESC\n";
		terminated = true;
    }
}





void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




void timf(int value)				// Timer function
{
	if(!terminated)					//if don't wanna terminate
	{
		glutPostRedisplay();		//Redraw windows
		glutTimerFunc(GL_TIMER_DELAY, timf, 0);	//Setup next timer
	}
	else							//if wanna terminate
	{
		terminated = false;			//set terminated flag
		exit(RETURN_CODE_OK);		//terminate thread
	}
}




int main(int argc, char **argv)
{
	obstacles = (struct obstacle*)malloc(sizeof(struct obstacle) * numOfObstacles);
	obstacles[0] = createObstacle(20, 20, 20);
	obstacles[1] = createObstacle(50, 50, 20);
	obstacles[2] = createObstacle(80, 80, 20);
	obstacles[3] = createObstacle(110, 110, 20);

	startway.x 	= 0;
	startway.y 	= 0;
	endway.x	= 150;
	endway.y	= 150;

 	cout << "glut init\n";
	glutInit(&argc, argv);
	cout << "dispmode\n";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("floating");
	cout << "reshape\n";
	glutReshapeFunc(reshape);
	cout << "dispfunc\n";
	glutDisplayFunc(renderScene);
    cout << "keyfunc\n";
    glutKeyboardFunc(handleKeypress);
	cout << "timer\n";
	glutTimerFunc(GL_TIMER_DELAY, timf, 0);
	cout << "mainloop\n";
	glutMainLoop();


	return 1;
}
