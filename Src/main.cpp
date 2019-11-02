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
	int ret = ((MIN_DATASEIZE_FUNC_A * pow((double)calculatedPoints, 2)) + (MIN_DATASEIZE_FUNC_B * (calculatedPoints)) + MIN_DATASEIZE_FUNC_C) / 4;
	if(ret < 2)
		ret = 2;
	if(ret >= numOfObstacles)
		ret = (numOfObstacles) - 1;

	cout << "min size for " << (int)(calculatedPoints) << ": " << (int)ret << endl;
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


struct array* getDataSet(const struct coords *c)
{
	size_t size = 0;
	uint16_t watchRadius = ZERO_QUAD_CHECK_SIDE;
	while (size < getMinDatasetSize()) {
		size = 0;
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			if(obstacles[i].c->x + obstacles[i].a >= c->x - watchRadius
			&& obstacles[i].c->x <= c->x + obstacles[i].a + watchRadius
			&& obstacles[i].c->y + obstacles[i].a >= c->y - watchRadius
			&& obstacles[i].c->y <= c->y + obstacles[i].a + watchRadius)
				size++;
		}
		if(size < getMinDatasetSize())
		{
			cout << "Increacing radius\n";
			watchRadius += 100;
		}
	}
	cout << "Size: " << size << endl;
	size_t iter = 0;
	struct obstacle **obsts = (struct obstacle**)malloc(sizeof(struct obstacle*) * size);
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		if(obstacles[i].c->x + obstacles[i].a >= c->x - watchRadius
		&& obstacles[i].c->x <= c->x + obstacles[i].a + watchRadius
		&& obstacles[i].c->y + obstacles[i].a >= c->y - watchRadius
		&& obstacles[i].c->y <= c->y + obstacles[i].a + watchRadius)
			obsts[iter++] = &(obstacles[i]);
	}
	struct array *ret = (struct array*)malloc(sizeof(struct array));
	ret->items = *obsts;
	ret->size = size;
	return ret;
}


struct array* getDataSet(const struct obstacle *obst, const uint8_t corner)
{
	struct graphPoint *p = getPoint(obst, corner);
	struct coords *c = getCoordsOfCorner(obst, corner);

	struct array *ret = getDataSet(c);

	free(c);

	return ret;
}


bool hasIntersections(const struct array *dataset, const struct vect *v)
{
	for(size_t i = 0; i < dataset->size; i++)
	{
		if(hasIntersection(&((struct obstacle*)dataset->items)[i], v))
			return true;
	}
	return false;
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

struct coords getIntersection(const struct array *dataset, const struct vect *v)
{
	struct obstacle *o = NULL;
	for(size_t i = 0; i < dataset->size; i++)
	{
		if(hasIntersection(&((struct obstacle*)dataset->items)[i], v))
		{
			o = &((struct obstacle*)dataset->items)[i];
			break;
		}
	}
	if(o != NULL)
		return {0, 0};

	return getIntersection(o, v);
}


struct obstacle*	getIntersected(const struct array *dataset, const struct vect *v)
{
	struct obstacle *o = NULL;
	struct coords	inter;
	COORDS_DATATYPE l = INF;
	for(size_t i = 0; i < dataset->size; i++)
	{
		inter = getIntersection(&((struct obstacle*)dataset->items)[i], v);
		COORDS_DATATYPE _l = getLen(v->c, &inter);
		if(inter.x != -1 && inter.y != -1 && _l < l)
		{
			o = &((struct obstacle*)dataset->items)[i];
			l = _l;
		}
	}
	return o;
}

void initPoint(struct graphPoint *p, const struct coords *c, const struct array *obsts)
{
	struct vect v;
	struct coords *tgt;
	for(size_t i = 0; i < obsts->size; i++)
	{
		for(uint8_t j = 0; j < 4; j++)
		{
			tgt = getCoordsOfCorner(&((struct obstacle*)obsts->items)[i], j);
			// tgt = &((struct obstacle*)obsts->items)[i].corners[j]->c;
			v = createVect(c, tgt);
			if((v.dx != 0 || v.dy != 0) && !hasIntersections(&v))
			{
				// cout << "Creating tgt\n";
				if(p->numOfTargets == 0)
					calculatedPoints++;
				if(((struct obstacle*)obsts->items)[i].corners[j]->numOfTargets == 0)
					calculatedPoints++;
				cout << "Creating from " << p->c.toString() << " to " << ((struct obstacle*)(obsts->items))[i].corners[j]->c.toString() << endl;
				addTarget(p, ((struct obstacle*)(obsts->items))[i].corners[j]);
				for(uint8_t h = 0; h < 4; h++)
					if(((struct obstacle*)obsts->items)[i].corners[h]->numOfTargets == 0)
						initPoint(((struct obstacle*)obsts->items)[i].corners[h], getCoordsOfCorner(&((struct obstacle*)obsts->items)[i], h));
				// initObst(&((struct obstacle*)obsts->items)[i]);
			}
			else
			{
				cout << "AAA\n" << v.dx << ":" << v.dy << endl;
			}
			free(tgt);
		}
	}
}



void initPoint(struct graphPoint *p, const struct coords *c)
{
	struct array *obsts = getDataSet(c);
	initPoint(p, c, obsts);
	free(obsts);
}


void initObstCorner(const struct obstacle *obst, const uint8_t corner)
{
	cout << "Watching corner " << (int)corner << " of obstacle " << obst << endl;
	struct graphPoint *point = getPoint(obst, corner);
	struct coords *c = getCoordsOfCorner(obst, corner);
	initPoint(point, c);
}



void initObst(const struct obstacle *obst)
{
	for(uint8_t i = 0; i < 4; i++)
		initObstCorner(obst, i);
}



void initGraph(struct graphPoint *startp, const struct coords *startc)
{
	struct array *ar = getDataSet(startc);
	initPoint(startp, startc, ar);
	for(size_t i = 0; i < graphSize; i++)
	{
		if(graph[i]->numOfTargets == 0)
		{
			free(ar);
			ar = getDataSet(&graph[i]->c);
			initPoint(graph[i], &graph[i]->c, ar);
		}
	}
}








struct coords getCoordsOfPoint(struct graphPoint *p)
{
	return (p->c);
}







void calculateWay(size_t index) 
{
	if(graph[index]->calculated || graph[index] == NULL)
		return;
	graph[index]->calculated = true;
	for(size_t i = 0; i < graph[index]->numOfTargets; i++)
	{
		COORDS_DATATYPE w = getWayPrice(graph[index], graph[index]->targets[i]);
		if(graph[index]->targets[i]->weight > w + graph[index]->weight)
		{
			graph[index]->targets[i]->weight = w + graph[index]->weight;
			ways[graph[index]->targets[i]->i] = index;
		}
		if(!graph[index]->targets[i]->calculated)
			calculateWay(graph[index]->targets[i]->i);
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


void drawWeight(struct graphPoint *start, struct graphPoint *end)
{
	COORDS_DATATYPE w = getWayPrice(start, end),
					x = end->c.x - start->c.x,
					y = end->c.y - start->c.y;
	drawText(to_wstring(w), 12, x, y);
}


void drawWeight(struct coords *start, struct coords *end)
{
	glSetColor(0xFFFFFF);
	COORDS_DATATYPE w = getWayPrice(start, end),
					x = (end->x + start->x) / 2,
					y = (end->y + start->y) / 2;
	drawText(to_wstring(w), 10, x, y);
}



void drawEdges(struct graphPoint *p)
{
	if(p == NULL || p->numOfTargets == 0) 
	{
		cout << "SHIT\n";
		return;
	}

	struct coords start, end;
	start = getCoordsOfPoint(p);
	for(size_t i = 0; i < p->numOfTargets; i++)
	{
		end = getCoordsOfPoint(p->targets[i]);
		glSetColor(GRAPH_COLOR);
		drawLine(&start, &end);
		// drawWeight(&start, &end);

		cout 	<< "\tLine: " << endl
				<< "\t\t" << start.x << ":" << start.y << " -> " << end.x << ":" << end.y << endl;
	}
}




void drawObstacle(const struct obstacle *o)
{
	// cout << "Draw obst: x=" << o->c->x << ", y=" << o->c->y << ", a=" << o->a << endl;
	glSetColor(OBST_COLOR);
	drawQuad(o->c->x, o->c->y, o->a);
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
			drawText(to_wstring(i), 12, obstacles[i].c->x + obstacles[i].a / 2, obstacles[i].c->y + obstacles[i].a / 2);
		}
		for(size_t i = 0; i < graphSize; i++)
		{
			glLineWidth(1);
			// cout << "edge from:{" << graph[i]->c->x << ":" << graph[i]->c->y << "} to {" << endl;
			drawEdges(graph[i]);
		}

		cout << "{";
		for(int i = 0; i < graphSize; i++)
			cout << i << ((i < graphSize - 1)? ", " : " ");
		cout << "}\n";
		cout << "{";
		for(int i = 0; i < graphSize; i++)
			cout << ways[i] << ((i < graphSize - 1)? ", " : " ");
		cout << "}\n";

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
	obstacles[iter++] = createObstacle(280, 580, 20);
	obstacles[iter++] = createObstacle(60, 400, 30);
	numOfObstacles = iter;

	graphSize = numOfObstacles * 4 + 2;

	struct graphPoint *p = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c;
	c.x = 350;
	c.y = 10;
	p->c = c;
	struct graphPoint *p2 = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c2;
	c2.x = 170;
	c2.y = 320;
	p2->c = c2;

	graph = (struct graphPoint**)malloc(graphSize * sizeof(struct graphPoint*));
	target = numOfObstacles * 4;
	home = target + 1;
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

	struct graphPoint startp, endp;
	startp.c.x = 300;
	startp.c.y = 100;
	endp.c.x = 300;
	endp.c.y = 500;
	// struct vect v = createVect(&startp.c, &endp.c);
	struct vect v = createVect(&obstacles[0].corners[2]->c, &obstacles[1].corners[3]->c);
	cout << v.toString() << endl << v.c->toString() << endl << "{" << (v.dx + v.c->x) << ":" << (v.dy + v.c->y) << "}" << endl;
	cout << getIntersection(&obstacles[0], &v).toString() << endl;
	// cout << hasIntersections(&v) << endl;
	cout << hasIntersection(&obstacles[0], &v) << endl;
	// for(int i = 0; i < numOfObstacles; i++)
	// {
	// 	addTarget(obstacles[i].corners[0], obstacles[i].corners[1]);
	// 	addTarget(obstacles[i].corners[1], obstacles[i].corners[2]);
	// 	addTarget(obstacles[i].corners[2], obstacles[i].corners[3]);
	// 	addTarget(obstacles[i].corners[3], obstacles[i].corners[0]);
	// }
	// exit(0);
	initGraph(graph[numOfObstacles * 4], &c3);
	calculateWay(target);


	


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
