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
#include "grapher.hpp"
#include "pathfinder.hpp"


#define GL_TIMER_DELAY	400
#define RETURN_CODE_OK 0
#define OBST_COLOR 	0x268BD2
#define GRAPH_COLOR 0x99D4AA
#define DATA_COLOR	0xD9E6F2
#define WAY_COLOR	0xAA99D4
#define BG_COLOR	0x1C2639

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
	drawText(L"0", 10, o->c->x + 5, o->c->y + 5);
	drawText(L"1", 10, o->c->x + 5, o->c->y + o->b - 15);
	drawText(L"2", 10, o->c->x + o->a - 15, o->c->y + o->b - 15);
	drawText(L"3", 10, o->c->x + o->a - 15, o->c->y + 5);
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
		glSetColor(BG_COLOR);
		drawRect(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
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
		glSetColor(WAY_COLOR);
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

	int W = 0;
	for(int i = 0; i < graphSize; i++)
		if(graph[i]->c.x > W)
			W = graph[i]->c.x;

	int H = 0;
	for(int i = 0; i < graphSize; i++)
		if(graph[i]->c.x > H)
			H = graph[i]->c.x;
	

	


 	cout << "glut init\n";
	glutInit(&argc, argv);
	cout << "dispmode\n";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("floating");
	cout << "reshape\n";
	glutReshapeWindow(W, H);
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
