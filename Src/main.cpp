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

#define GL_TIMER_DELAY	400
#define RETURN_CODE_OK 0
#define OBST_COLOR 0xFF0000
#define GRAPH_COLOR 0x00FFFF
#define DATA_COLOR	0xFFFFFF

// #define VECTORS_PER_CHECK	50

#define ZERO_QUAD_CHECK_SIDE 2000;
size_t watchRadius = ZERO_QUAD_CHECK_SIDE;

#define MIN_DATASEIZE_FUNC_A	0.0
#define MIN_DATASEIZE_FUNC_B	0.0
#define MIN_DATASEIZE_FUNC_C	numOfObstacles

#define FIELD_MAX_LEN		2000

using namespace std;


bool terminated = false;
size_t numOfObstacles = 0;
size_t graphSize = 0;
size_t calculatedPoints = 0;


struct obstacle *obstacles;
struct coords	startway,
				endway;
struct graphPoint **graph;
size_t *ways;

size_t 	target = 1,
		home = 1;


struct array
{
	void *items;
	size_t size;
};


uint8_t getMinDatasetSize()
{
	int ret = numOfObstacles;
	if(ret < 2)
		ret = 2;
	if(ret >= numOfObstacles)
		ret = (numOfObstacles) - 1;
	return ret;
}




void initObst(const struct obstacle *obst);
void initPoint(struct graphPoint *p, const struct coords *c);



/*
 *	-------+----->	0
 *	<------+------	180
 *		  /|\
 *         |		90
 *		   +
 */
struct vect createVect(const struct coords *c, const COORDS_DATATYPE angle)
{
	struct vect ret;
	ret.c  = (struct coords*)malloc(sizeof(struct coords));
	ret.c->x = c->x;
	ret.c->y = c->y;
	ret.dx = FIELD_MAX_LEN * 10.0 / tan(angle / 180.0 * PI);
	ret.dy = ret.dx * tan(angle / 180.0 * PI);
	ret.dx = abs(ret.dx);
	ret.dy = abs(ret.dy);
	if(angle > 180 || angle < 0)
		ret.dy *= -1;

	if(!((angle >= -90 && angle <= 90) || (angle >= 190 + 90 && angle <= 360 + 90)))
		ret.dx *= -1;
	return ret;
}

struct vect createVect(const struct coords *start, const struct coords *end)
{
	struct vect ret;
	ret.c = (struct coords*)malloc(sizeof(struct coords));
	ret.c->x = start->x;
	ret.c->y = start->y;
	ret.dx = end->x - start->x;
	ret.dy = end->y - start->y;
	return ret;
}


struct vect createVect(const struct obstacle *obst, const uint8_t corner, const COORDS_DATATYPE angle)
{
	struct vect ret;
	struct coords *c = getCoordsOfCorner(obst, corner);
	ret = createVect(c, angle);
	free(c);
	return ret;
}


struct array* getPointsDataSet(const struct coords *c)
{
	size_t size = 0;
	uint16_t watchRadius = ZERO_QUAD_CHECK_SIDE;
	while (size < getMinDatasetSize()) {
		size = 0;
		for(size_t i = 0; i < graphSize; i++)
		{
			if(graph[i]->c.x >= c->x - watchRadius
			&& graph[i]->c.x <= c->x + watchRadius
			&& graph[i]->c.y >= c->y - watchRadius
			&& graph[i]->c.y <= c->y + watchRadius)
				size++;
		}
		if(size < getMinDatasetSize())
		{
			cout << "Increacing radius\n";
			watchRadius += 100;
		}
	}
	size_t iter = 0;
	struct graphPoint **dataset = (struct graphPoint**)malloc(sizeof(struct graphPoint*) * size);
	for(size_t i = 0; i < graphSize; i++)
	{
		if(graph[i]->c.x >= c->x - watchRadius
		&& graph[i]->c.x <= c->x + watchRadius
		&& graph[i]->c.y >= c->y - watchRadius
		&& graph[i]->c.y <= c->y + watchRadius)
			dataset[iter++] = graph[i];
	}
	struct array *ret = (struct array*)malloc(sizeof(struct array));
	ret->items = dataset;
	ret->size = size;
	return ret;
}



bool hasIntersections(const struct vect *v)
{
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		if(hasIntersection(&obstacles[i], v))
			return true;
	}
	return false;
}


#define CONST 0.4

bool isDotInside(const struct coords *c)
{
    for(size_t i = 0; i < numOfObstacles; i++)
	{
		if(	(c->x - CONST > obstacles[i].c->x)
		&&  (c->x + CONST < obstacles[i].c->x + obstacles[i].a)
		&&  (c->y - CONST > obstacles[i].c->y)
		&&  (c->y + CONST < obstacles[i].c->y + obstacles[i].a))
			return true;
	}
	return false;
}





void initPoint(struct graphPoint *p)
{
	struct array* points = getPointsDataSet(&p->c);
	struct vect v;
	struct coords *tgt;
	struct graphPoint *currP = 0x00;
	for(int i = 0; i < points->size; i++)
	{
		currP = ((struct graphPoint**)(points->items))[i];
		v = createVect(&p->c, &currP->c);
		if((v.dx != 0 || v.dy != 0) && !hasIntersections(&v) && !isDotInside(&p->c) && !isDotInside(&currP->c))
		{
			if(p->numOfTargets == 0)
				calculatedPoints++;
			if(currP->numOfTargets == 0)
				calculatedPoints++;
			addTarget(p, currP);
			if(currP->numOfTargets == 1)
				initPoint(currP);
		}
	}
	free(points);
}













struct coords getCoordsOfPoint(struct graphPoint *p)
{
	return (p->c);
}





bool hasUncalculatedPoints(const struct graphPoint *p)
{
	for(size_t i = 0; i < (p->numOfTargets); i++)
	{
		if(!(p->targets[i]->calculated))
			return true;
	}
	return false;
}

struct graphPoint* getMinTarget(const struct graphPoint *p)
{
	COORDS_DATATYPE w = INF;
	struct graphPoint * pp = 0;
	for(size_t i = 0; i < p->numOfTargets; i++)
	{
		if(!p->targets[i]->calculated && p->targets[i]->weight <= w)
		{
			w = p->targets[i]->weight;
			pp = p->targets[i];
		}
	}
	return pp;
}

void calculateWay(size_t index) 
{
	if(graph[index] == NULL || graph[index]->calculated)
		return;
	graph[index]->calculated = true;
	
	if(graph[index]->numOfTargets == 0)
		return;

	struct graphPoint * watch;
	COORDS_DATATYPE w;

	for(size_t i = 0; i < graph[index]->numOfTargets; i++)
	{
		watch = graph[index]->targets[i];
		w = getWayPrice(graph[index], watch);
		if(watch->weight > graph[index]->weight + w)
		{
			watch->weight = w + graph[index]->weight;
			ways[watch->i] = index;
		}
	}

	while (hasUncalculatedPoints(graph[index]))
	{
		watch = getMinTarget(graph[index]);
		calculateWay(watch->i);
	}
}



void drawLine(struct coords *start, struct coords *end)
{
	drawLine(start->x, start->y, end->x, end->y);
}

void drawLine(struct graphPoint *start, struct graphPoint *end)
{
	drawLine(&start->c, &end->c);
}


// void drawWeight(struct graphPoint *start, struct graphPoint *end)
// {
// 	COORDS_DATATYPE w = getWayPrice(start, end),
// 					x = end->c.x - start->c.x,
// 					y = end->c.y - start->c.y;
// 	drawText(to_wstring(w), 12, x, y);
// }


// void drawWeight(struct coords *start, struct coords *end)
// {
// 	glSetColor(0xFFFFFF);
// 	COORDS_DATATYPE w = getWayPrice(start, end),
// 					x = (end->x + start->x) / 2,
// 					y = (end->y + start->y) / 2;
// 	drawText(to_wstring(w), 10, x, y);
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
		drawLine(&p->c, &p->targets[i]->c);
	}
}




void drawObstacle(const struct obstacle *o)
{
	// cout << "Draw obst: x=" << o->c->x << ", y=" << o->c->y << ", a=" << o->a << endl;
	glSetColor(OBST_COLOR);
	// drawQuad(o->c->x, o->c->y, o->a);
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
	obstacles = (struct obstacle*)malloc(sizeof(struct obstacle) * numOfObstacles);
	obstacles[iter++] = createObstacle(100, 100, 200);
	obstacles[iter++] = createObstacle(110, 350, 200);
	obstacles[iter++] = createObstacle(510, 350, 200, 100);
	// obstacles[iter++] = createObstacle(280, 580, 20);
	// obstacles[iter++] = createObstacle(80, 330, 50);
	// obstacles[iter++] = createObstacle(120, 340, 50);
	// obstacles[iter++] = createObstacle(60, 400, 30);
	numOfObstacles = iter;

	graphSize = numOfObstacles * 4 + 2;

	struct graphPoint *p = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c;
	c.x = 350;
	c.y = 10;
	p->c = c;
	struct graphPoint *p2 = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c2;
	c2.x = 80;
	c2.y = 440;
	p2->c = c2;

	graph = (struct graphPoint**)malloc(graphSize * sizeof(struct graphPoint*));
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


	initPoint(p);
	calculateWay(target);
	
	

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
