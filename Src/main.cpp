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

#include "bases.hpp"
#include "routines.hpp"
#include "findwayBases.hpp"
#include "grapher.hpp"
#include "pathfinder.hpp"


#define GL_TIMER_DELAY	400
#define RETURN_CODE_OK 0
#define OBST_COLOR 0xFF0000
#define GRAPH_COLOR 0x00FFFF
#define DATA_COLOR	0xFFFFFF

using bases::array;
using bases::coords;
using bases::obstacle;

using namespace std;


bool terminated = false;










void drawLine(struct coords *start, struct coords *end)
{
	drawLine(start->x, start->y, end->x, end->y);
}

void drawLine(struct bases::graphPoint *start, struct bases::graphPoint *end)
{
	drawLine(&start->c, &end->c);
}





void drawEdges(struct bases::graphPoint *p)
{
	if(p == NULL || p->numOfTargets == 0) 
	{
		return;
	}
	glSetColor(GRAPH_COLOR);
	for(size_t i = 0; i < p->numOfTargets; i++)
	{
		drawLine(&p->c, &p->targets[i]->c);
	}
}




void drawObstacle(const struct obstacle *o)
{
	glSetColor(OBST_COLOR);
	drawRect(o->c->x, o->c->y, o->a, o->b);
	glSetColor(DATA_COLOR);
	drawText(L"0", 10, o->c->x, o->c->y);
	drawText(L"1", 10, o->c->x, o->c->y + o->b);
	drawText(L"2", 10, o->c->x + o->a, o->c->y + o->b);
	drawText(L"3", 10, o->c->x + o->a, o->c->y);
}


void drawWay(size_t pointNum, size_t target)
{
	size_t curr = pointNum;
	while(curr != target)
	{
		drawLine(graph[curr], graph[ways[curr]]);
		curr = ways[curr];
	}
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

		for(size_t i = 0; i < numOfObstacles; i++)
		{
			drawObstacle(&obstacles[i]);
			glSetColor(DATA_COLOR);
			drawText(to_wstring(i), 12, obstacles[i].c->x + obstacles[i].a / 2, obstacles[i].c->y + obstacles[i].b / 2);
		}
		for(size_t i = 0; i < graphSize; i++)
		{
			glLineWidth(1);
			drawEdges(graph[i]);
		}

		glLineWidth(5);
		glSetColor(0x00FF99);
		drawWay(home, target);

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
	numOfObstacles = 30;
	size_t iter = 0;
	obstacles = (struct bases::obstacle*)malloc(sizeof(struct bases::obstacle) * numOfObstacles);
	obstacles[iter++] = bases::createObstacle(100, 100, 200);
	obstacles[iter++] = bases::createObstacle(110, 350, 200);
	obstacles[iter++] = bases::createObstacle(510, 350, 200, 100);
	// obstacles[iter++] = createObstacle(280, 580, 20);
	// obstacles[iter++] = createObstacle(80, 330, 50);
	// obstacles[iter++] = createObstacle(120, 340, 50);
	// obstacles[iter++] = createObstacle(60, 400, 30);
	numOfObstacles = iter;

	graphSize = numOfObstacles * 4 + 2;

	struct bases::graphPoint *p = (struct bases::graphPoint*)malloc(sizeof(struct bases::graphPoint));

	struct coords c;	
	c.x = 350;
	c.y = 10;
	p->c = c;
	struct bases::graphPoint *p2 = (struct bases::graphPoint*)malloc(sizeof(struct bases::graphPoint));

	struct coords c2;
	c2.x = 80;
	c2.y = 440;
	p2->c = c2;

	graph = (struct bases::graphPoint**)malloc(graphSize * sizeof(struct bases::graphPoint*));
	target = numOfObstacles * 4;
	home = target + 1;
	p2->i = home;
	p->i = target;
	graph[target] = p;
	graph[home] = p2;
	for(size_t i = 0; i < numOfObstacles * 4; i++)
		graph[i] = obstacles[i / 4].corners[i % 4];

	for(size_t i = 0; i < graphSize; i++)
	{
		graph[i]->i = i;
		graph[i]->weight = (1.0 / 0.0);
	}
	graph[target]->weight = 0;

	ways = (size_t*)malloc(graphSize * sizeof(size_t));
	for(size_t i = 0; i < graphSize; i++)
		ways[i] = target;

	startway.x 	= 0;
	startway.y 	= 0;
	endway.x	= 150;
	endway.y	= 150;


	struct coords c3;
	c3.x = 200;
	c3.y = 0;


	grapher::initPoint(p);
	pathfinder::calculateWay(target);
	
	

	if(argc == 1)
		exit(0);

	


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


	return 0;
}
