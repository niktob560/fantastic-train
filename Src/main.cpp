#include <iostream>
#include <iomanip>
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
#include "geometry.hpp"
#include "pathfinder.hpp"

#include "manager.hpp"


#define GL_TIMER_DELAY	400
#define RETURN_CODE_OK 0
#define OBST_COLOR 	0x268BD2
#define GRAPH_COLOR 0x99D4AA
#define DATA_COLOR	0xD9E6F2
#define WAY_COLOR	0xAA99D4
#define BG_COLOR	0x1C2639
#define DATA_EXTRA_COLOR 0xDC322F

using graphbases::array;
using graphbases::coords;
using bases::obstacle;
using bases::graph;
using bases::ways;
using bases::graphSize;
using bases::numOfObstacles;
using bases::obstacles;
using bases::home;
using bases::target;
using bases::startway;
using bases::endway;
using graphbases::graphPoint;

using namespace std;


bool terminated = false;






void drawLine(struct coords *start, struct coords *end)
{
	drawLine(start->x, start->y, end->x, end->y);
}

struct obstacle* getRootObstacle(struct graphPoint *p)
{
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			if(obstacles[i].corners[j]->i == p->i)
				return &obstacles[i];
		}
	}
	return NULL;
}


void drawLine(struct graphPoint *start, struct graphPoint *end)
{
	struct coords 	c1 = geometry::getCoordsOfPoint(start), 
					c2 = geometry::getCoordsOfPoint(end);
	drawLine(&c1, &c2);
}





void drawEdges(struct graphPoint *p)
{
	if(p == NULL || p->numOfTargets == 0) 
	{
		return;
	}
	glSetColor(GRAPH_COLOR);
	for(size_t i = 0; i < p->numOfTargets; i++)
	{
		glSetColor(GRAPH_COLOR);
		drawLine(p, p->targets[i]);
	}
}




void drawObstacle(const struct obstacle *o)
{
	glSetColor(OBST_COLOR);
	drawRect(o->c->x, o->c->y, o->a, o->b, o->rot);
}


void drawWay(size_t pointNum, size_t target)
{
	size_t curr = pointNum;
	while(curr != target)
	{
		glSetColor(WAY_COLOR);
		drawLine(graph[curr], graph[ways[curr]]);
		curr = ways[curr];
	}
}

void drawDots()
{
	for(size_t i = 0; i < graphSize; i++)
	{
		glSetColor(DATA_EXTRA_COLOR);
		drawCircleFilled(geometry::getCoordsOfPoint(graph[i]).x, geometry::getCoordsOfPoint(graph[i]).y, 3, 10);
		glSetColor(DATA_COLOR);
		std::wstring str = L"";
		for(size_t k = 0; k < numOfObstacles; k++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(obstacles[k].corners[j]->i == graph[i]->i)
				{
					str = to_wstring(j);
					k = numOfObstacles;
					break;
				}
			}
		}
		drawText((str), 12, geometry::getCoordsOfPoint(graph[i]).x, geometry::getCoordsOfPoint(graph[i]).y);
	}
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
		glSetColor(BG_COLOR);
		drawRect(static_cast<float>(glutGet(GLUT_SCREEN_WIDTH) / 2.0), static_cast<float>(glutGet(GLUT_SCREEN_HEIGHT) / 2.0), static_cast<float>(glutGet(GLUT_SCREEN_WIDTH)), static_cast<float>(glutGet(GLUT_SCREEN_HEIGHT)));
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			drawObstacle(&obstacles[i]);
			glSetColor(DATA_COLOR);
			drawText(to_wstring(i), 12, obstacles[i].c->x, obstacles[i].c->y);
		}
		for(size_t i = 0; i < graphSize; i++)
		{
			glLineWidth(1);
			drawEdges(graph[i]);
		}

		glLineWidth(5);
		glSetColor(WAY_COLOR);
		drawWay(home, target);

		glSetColor(DATA_EXTRA_COLOR);
		drawText(to_wstring(static_cast<unsigned int>(graph[home]->weight)), 14, 10, 10);

		drawDots();

    glutSwapBuffers();
}


void handleKeypress(unsigned char key, //The key that was pressed
                                        int x, int y) //The current mouse coordinates
{
    x++;
    y++;
    cout << "PRESSED\n" << static_cast<int>(key) << endl;
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
    value++;
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
	numOfObstacles = 50;
	size_t iter = 0;
	graphSize = 2;
	graph = static_cast<struct graphPoint**>(malloc((200) * sizeof(struct graphPoint*)));
	obstacles = static_cast<struct bases::obstacle*>(malloc(sizeof(struct bases::obstacle) * numOfObstacles));
	obstacles[iter++] = bases::createObstacle(-5, 1020 / 2, 10, 1020);
	obstacles[iter++] = bases::createObstacle(1020, 1020 / 2, 10, 1020);
	obstacles[iter++] = bases::createObstacle(1020 / 2, -5, 1020, 10);
	obstacles[iter++] = bases::createObstacle(1020 / 2, 1020, 1020, 10);

	obstacles[iter++] = bases::createObstacle(100, 100, 100, 100, static_cast<float>(-M_PI_4 / 2.0));
	obstacles[iter++] = bases::createObstacle(310, 350, 200, 300, static_cast<float>(M_PI_4 * 1.0));
	obstacles[iter++] = bases::createObstacle(280, 580, 20, 20, static_cast<float>(2.0));
	obstacles[iter++] = bases::createObstacle(80, 330, 50, 40, static_cast<float>(4.0));
	obstacles[iter++] = bases::createObstacle(120, 340, 50, 49, static_cast<float>(22.0));
	obstacles[iter++] = bases::createObstacle(60, 400, 30, 32, static_cast<float>(5.0));
	obstacles[iter++] = bases::createObstacle(710, 400, 200, 100, static_cast<float>(1.8));
	obstacles[iter++] = bases::createObstacle(910, 500, 200, 100, static_cast<float>(1.8));
	obstacles[iter++] = bases::createObstacle(910, 500, 200, 100, static_cast<float>(1.8));
	obstacles[iter++] = bases::createObstacle(910, 100, 150, 100, static_cast<float>(1.8));
	obstacles[iter++] = bases::createObstacle(110, 500, 200, 100, static_cast<float>(1.8));
	obstacles[iter++] = bases::createObstacle(10, 600, 200, 100, static_cast<float>(3.8));
	obstacles[iter++] = bases::createObstacle(500, 400, 200, 100, static_cast<float>(2.8));
	obstacles[iter++] = bases::createObstacle(650, 300, 200, 100, static_cast<float>(2.8));
	obstacles[iter++] = bases::createObstacle(350, 700, 200, 100, static_cast<float>(0.8));
	obstacles[iter++] = bases::createObstacle(150, 600, 200, 100, static_cast<float>(0.2));
	obstacles[iter++] = bases::createObstacle(550, 550, 200, 100, static_cast<float>(0.4));
	obstacles[iter++] = bases::createObstacle(750, 550, 200, 100, static_cast<float>(0.9));
	numOfObstacles = iter;


	struct graphPoint *p = static_cast<struct graphPoint*>(malloc(sizeof(struct graphPoint)));

	p->i = 0;
	graph[0] = p;
	struct graphPoint *p2 = static_cast<struct graphPoint*>(malloc(sizeof(struct graphPoint)));

	p2->i = 1;
	graph[1] = p2;

	for(size_t i = 0; i < graphSize; i++)
		graph[i]->weight = INF;
		
	graph[target]->weight = 0;
	

	ways = static_cast<size_t*>(malloc(graphSize * sizeof(size_t)));
	for(size_t i = 0; i < graphSize; i++)
		ways[i] = target;

	geometry::targetCoords = {500, 1000};
	geometry::homeCoords = {20, 20};


	grapher::initPoint(p);
	pathfinder::calculateWay(target);
	

	cout << endl;
	for(size_t i = 0; i < graphSize; i++)
		cout << setw(3) << i;
	cout << endl;
	for(size_t i = 0; i < graphSize; i++)
		cout << setw(3) << ways[i];
	cout << endl;


	exit(0);


	int W = 0;
	for(size_t i = 0; i < graphSize; i++)
		if(geometry::getCoordsOfPoint(graph[i]).x > W)
			W = geometry::getCoordsOfPoint(graph[i]).x;
	for(size_t i = 0; i < numOfObstacles; i++)
		if(obstacles[i].c->x + obstacles[i].a > W)
			W = obstacles[i].c->x + obstacles[i].a;

	int H = 0;
	for(size_t i = 0; i < graphSize; i++)
		if(geometry::getCoordsOfPoint(graph[i]).y > H)
			H = geometry::getCoordsOfPoint(graph[i]).y;
	
	for(size_t i = 0; i < numOfObstacles; i++)
		if(obstacles[i].c->y + obstacles[i].b > H)
			W = obstacles[i].c->y + obstacles[i].b;

	

 	cout << "glut init\n";
	glutInit(&argc, argv);
	cout << "dispmode\n";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("floating");
	cout << "reshape\n";
	glutReshapeWindow(W - 20, H - 10);
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
