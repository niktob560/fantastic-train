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

// struct coords* getCoordsOfPoint(struct graphPoint *p)
// {
// 	struct obstacle *rootObst = NULL;
// 	size_t corner = 5;
// 	for(size_t i = 0; i < numOfObstacles; i++)
// 	{
// 		if(rootObst == NULL)
// 			for(size_t j = 0; j < 4; j++)
// 			{
// 				if(obstacles[i].corners[j]->i == p->i)
// 				{
// 					rootObst = &obstacles[i];
// 					corner = j;
// 					break;
// 				}
// 			}
// 		else
// 			break;
// 	}
// 	return geometry::getCoordsOfCorner(rootObst, bases::cornerFromNum(corner));
// }

void drawLine(struct graphPoint *start, struct graphPoint *end)
{
	struct coords 	c1 = geometry::getCoordsOfPoint(start), 
					c2 = geometry::getCoordsOfPoint(end);
	drawLine(&c1, &c2);
}

// void drawObstacleEdges(struct obstacle *o)
// {

// }





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
		// glSetColor(DATA_EXTRA_COLOR);
		// drawText(to_wstring(pathfinder::getWayPrice(p, p->targets[i])), 11, (p->c.x + p->targets[i]->c.x) / 2, (p->c.y + p->targets[i]->c.y) / 2);
	}
}




void drawObstacle(const struct obstacle *o)
{
	glSetColor(OBST_COLOR);
	// glPushMatrix();
	// glRotatef(o->rot, 0, 1, 0);
	drawRect(o->c->x, o->c->y, o->a - 2, o->b - 2, o->rot);
	// glPopMatrix();
	// glSetColor(DATA_COLOR);
	// drawText(L"0", 10, o->c->x, o->c->y);
	// drawText(L"1", 10, o->c->x, o->c->y + o->b);
	// drawText(L"2", 10, o->c->x + o->a, o->c->y + o->b);
	// drawText(L"3", 10, o->c->x + o->a, o->c->y);
}


void drawWay(size_t pointNum, size_t target)
{
	size_t curr = pointNum;
	while(curr != target)
	{
		glSetColor(WAY_COLOR);
		drawLine(graph[curr], graph[ways[curr]]);
		// glSetColor(DATA_EXTRA_COLOR);
		// drawText(to_wstring(pathfinder::getWayPrice(graph[curr], graph[ways[curr]])), 10, (graph[curr]->c.x + graph[ways[curr]]->c.x + 10) / 2, (graph[curr]->c.y + graph[ways[curr]]->c.y + 10) / 2);
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
		// drawText(to_wstring((int)graph[i]->i), 12, graph[i]->c.x - 10, graph[i]->c.y - 6);
		std::wstring str = L"";
		for(size_t k = 0; k < numOfObstacles; k++)
		{
			for(size_t j = 0; j < 4; j++)
			{
				// obs bases::cornerFromNum(j);
				if(obstacles[k].corners[j]->i == graph[i]->i)
				{
					str = to_wstring((int)j);
					k = numOfObstacles;
					break;
				}
			}
		}
		// str = to_wstring((int)graph[i]->i);
		drawText((str), 12, geometry::getCoordsOfPoint(graph[i]).x, geometry::getCoordsOfPoint(graph[i]).y);
	}
	// for(size_t i = 0; i < numOfObstacles; i++)
	// {
	// 	for(size_t j = 0; j < 4; j++)
	// 	{
	// 		glSetColor(DATA_EXTRA_COLOR);
	// 		// drawCircleFilled
	// 	}
	// }
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
		glSetColor(BG_COLOR);
		drawRect(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
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
		drawText(to_wstring(graph[home]->weight), 14, 10, 10);

		drawDots();

    glutSwapBuffers();
}


void handleKeypress(unsigned char key, //The key that was pressed
                                        int x, int y) //The current mouse coordinates
{
    x++;
    y++;
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
	numOfObstacles = 300;
	size_t iter = 0;
	graphSize = 2;
	graph = (struct graphPoint**)malloc((7 * 4 + 2) * sizeof(struct graphPoint*));
	obstacles = (struct bases::obstacle*)malloc(sizeof(struct bases::obstacle) * numOfObstacles);
	obstacles[iter++] = bases::createObstacle(100, 100, 100, 100, M_PI_4 / 2);
	obstacles[iter++] = bases::createObstacle(310, 350, 200, 300, M_PI_4);
	obstacles[iter++] = bases::createObstacle(280, 580, 20);
	obstacles[iter++] = bases::createObstacle(80, 330, 50);
	obstacles[iter++] = bases::createObstacle(120, 340, 50);
	obstacles[iter++] = bases::createObstacle(60, 400, 30);
	obstacles[iter++] = bases::createObstacle(710, 400, 200, 100);
	numOfObstacles = iter;

	// graphSize = numOfObstacles * 4 + 2;

	struct graphPoint *p = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	// struct coords c;	
	// c.x = 600;
	// c.y = 300;
	// p->c = c;
	p->i = 0;
	graph[0] = p;
	struct graphPoint *p2 = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	// struct coords c2;
	// c2.x = 100;
	// c2.y = 400;
	p2->i = 1;
	graph[1] = p2;
	// p2->c = c2;
	// target = 1;
	// home = 0;
	// target = numOfObstacles * 4;
	// home = target + 1;
	// p2 = home;
	// p = target;
	// graph[target] = p;
	// graph[home] = p2;
	// for(size_t i = 0; i < numOfObstacles * 4; i++)
	// 	graph[i] = obstacles[i / 4].corners[i % 4];

	for(size_t i = 0; i < graphSize; i++)
	{
		graph[i]->weight = INF;
	}
	graph[target]->weight = 0;
	

	ways = (size_t*)malloc(graphSize * sizeof(size_t));
	for(size_t i = 0; i < graphSize; i++)
		ways[i] = target;


	// bases::vect _v_ = {&c2, (c.x-c2.x), (c.y-c2.y)};
	// cout << geometry::hasIntersections(&_v_) << endl;

	struct obstacle nobst = obstacles[0];
	nobst.a -= 2;
	nobst.b -= 2;

	struct coords 	_c0 = geometry::getCoordsOfPoint(graph[0]), 
					_c1 = geometry::_getCoordsOfCorner(&obstacles[0], bases::cornerFromNum(1)),
					__c0 = geometry::_getCoordsOfCorner(&nobst, bases::cornerFromNum(2)), 
					__c1 = geometry::_getCoordsOfCorner(&nobst, bases::cornerFromNum(3));



	struct bases::vect 	_v = geometry::createVect(&_c0, &_c1),
						_v1 = geometry::createVect(&__c0, &__c1);

	cout << geometry::hasIntersection(&_v, &_v1) << endl;
	cout << geometry::hasIntersection(&obstacles[0], &_v) << endl;
	cout << geometry::hasIntersections(&_v) << endl;
	cout << "{" << __c0.x << ";" << __c0.y << "}->{" << __c1.x << ";" << __c1.y << "}\n";
	cout << "{" << _c0.x << ";" << _c0.y << "}->{" << _c1.x << ";" << _c1.y << "}\n";
	// cout << geometry::hasIntersection(&obstacles[5], &_v) << endl;
	// cout << geometry::hasIntersection(&obstacles[1], &_v) << endl;
	// cout << "{" << obstacles[5].c->x << ";" << obstacles[5].c->y << "}\t";
	// cout << "[" << obstacles[5].a << ";" << obstacles[5].b << "]" << endl;
	// exit(0);



	grapher::initPoint(p);
	pathfinder::calculateWay(target);
	

	cout << endl;
	for(size_t i = 0; i < graphSize; i++)
		cout << setw(3) << i;
	cout << endl;
	for(size_t i = 0; i < graphSize; i++)
		cout << setw(3) << ways[i];
	cout << endl;


	// int W = 0;
	// for(size_t i = 0; i < graphSize; i++)
	// 	if(graph[i]->c.x > W)
			// W = graph[i]->c.x;
	// for(size_t i = 0; i < numOfObstacles; i++)
	// 	if(obstacles[i].c->x + obstacles[i].a > W)
	// 		W = obstacles[i].c->x + obstacles[i].a;

	// int H = 0;
	// for(size_t i = 0; i < graphSize; i++)
	// 	if(graph[i]->c.y > H)
	// 		H = graph[i]->c.y;
	
	// for(size_t i = 0; i < numOfObstacles; i++)
	// 	if(obstacles[i].c->y + obstacles[i].b > H)
	// 		W = obstacles[i].c->y + obstacles[i].b;

	
	// while(1);

 	cout << "glut init\n";
	glutInit(&argc, argv);
	cout << "dispmode\n";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("floating");
	cout << "reshape\n";
	// glutReshapeWindow(W, H);
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
