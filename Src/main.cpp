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

#define VECTORS_PER_CHECK	50

size_t QUAD_CHECK_SIDE = 5;

using namespace std;


bool terminated = false;
size_t numOfObstacles = 0;
size_t graphSize = 0;
size_t calculatedPoints = 0;


struct obstacle *obstacles;
struct coords	startway,
				endway;
struct graphPoint **graph;



struct array
{
	void *items;
	size_t size;
};


uint8_t getMinDatasetSize()
{

}





void calculateWay()
{

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
	ret.dx = QUAD_CHECK_SIDE * 10.0 / tan(angle / 180.0 * PI);
	ret.dy = ret.dx * tan(angle / 180.0 * PI);
	ret.dx = abs(ret.dx);
	ret.dy = abs(ret.dy);
	if(angle > 180 || angle < 0)
		ret.dy *= -1;

	if(!((angle >= -90 && angle <= 90) || (angle >= 190 + 90 && angle <= 360 + 90)))
		ret.dx *= -1;

	// cout <<  "\tcreate vect:" << endl
	// 	 << "\tdx: " << ret.dx << endl
	// 	 << "\tdy: " << ret.dy << endl
	// 	 << "\tx:  " << ret.c->x << endl
	// 	 << "\ty:  " << ret.c->y << endl;
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
	while (size < 2) {
		size = 0;
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE
			&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE
			&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE
			&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE)
				size++;
		}
		if(size < 2)
		{
			cout << "Increacing radius\n";
			QUAD_CHECK_SIDE += 100;
		}
	}
	cout << "Size: " << size << endl;
	size_t iter = 0;
	struct obstacle **obsts = (struct obstacle**)malloc(sizeof(struct obstacle*) * size);
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		// struct coords *c = getCoordsOfCorner(obst, corner);
		if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE
		&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE
		&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE
		&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE)
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
		return {-1, -1};

	return getIntersection(o, v);
}


struct obstacle*	getIntersected(const struct array *dataset, const struct vect *v)
{
	struct obstacle *o = NULL;
	struct coords	inter;
	COORDS_DATATYPE l = INF;
	// cout << "Watching for vect " << v->c->x << ":" << v->c->y << "; d" << v->dx << ":d" << v->dy << endl;
	for(size_t i = 0; i < dataset->size; i++)
	{
		// cout << "###Watching for intersection, obst: " << (int)i << "\t\t###" << endl;
		inter = getIntersection(&((struct obstacle*)dataset->items)[i], v);
		COORDS_DATATYPE _l = getLen(v->c, &inter);
		if(inter.x != -1 && inter.y != -1 && _l < l)
		{
			// cout << "####Found intersection: " << inter.x << ":" << inter.y << " with len " << _l << "\t\t\t####" << endl;
			o = &((struct obstacle*)dataset->items)[i];
			l = _l;
		}
	}
	return o;
}

void initPoint(struct graphPoint *p, const struct coords *c, const struct array *obsts)
{
	struct vect v;
	// for(uint8_t i = 0; i < VECTORS_PER_CHECK; i++)
	// {
	// 	v = createVect(c, ((360.0 / VECTORS_PER_CHECK) * i));
	// 	struct obstacle *o = getIntersected(obsts, &v);
	// 	if(o != NULL)
	// 	{
	// 		struct coords *c;
	// 		for(uint8_t h = 0; h < 4; h++)
	// 		{
	// 			cout << "->Found intersection" << endl;
	// 			c = getCoordsOfCorner(o, h);
	// 			v.dx = (c->x - v.c->x);
	// 			v.dy = (c->y - v.c->y);
	// 			if((v.dx != 0 || v.dy != 0) && getIntersected(obsts, &v) == NULL)
	// 			{
	// 				addTarget(p, o->corners[h]);
	// 			}
	// 			free(c);
	// 		}
	// 	}
	// }



	struct coords *tgt;
	for(size_t i = 0; i < obsts->size; i++)
	{
		for(uint8_t j = 0; j < 4; j++)
		{
			tgt = getCoordsOfCorner(&((struct obstacle*)obsts->items)[i], j);
			v = createVect(c, tgt);
			if((v.dx != 0 || v.dy != 0) && !hasIntersections(&v))
			{
				cout << "Creating tgt\n";
				if(p->numOfTargets == 0)
					calculatedPoints++;
				if(((struct obstacle*)obsts->items)[i].corners[j]->numOfTargets == 0)
					calculatedPoints++;
				addTarget(p, ((struct obstacle*)obsts->items)[i].corners[j]);
				for(uint8_t h = 0; h < 4; h++)
					if(((struct obstacle*)obsts->items)[i].corners[h]->numOfTargets == 0)
						initPoint(((struct obstacle*)obsts->items)[i].corners[h], getCoordsOfCorner(&((struct obstacle*)obsts->items)[i], h));
				// initObst(&((struct obstacle*)obsts->items)[i]);
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
}











struct coords getCoordsOfPoint(const struct graphPoint *p)
{
	return p->c;

	for(size_t i = 0; i < numOfObstacles; i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			if(p == getPoint(&obstacles[i], j))
			{
				cout << "\t\t\tPoint found: obst: " << i << endl
					 << "\t\t\tCorner: " << j << endl
					 << "\t\tobst: " << &obstacles[i] << " corners: ";
				for(int s = 0; s < 4; s++)
					cout << obstacles[i].corners[s] << " ";
				cout << endl << "\t\tCurr: " << p << endl;
				return *getCoordsOfCorner(&obstacles[i], j);
			}
		}
	}

	return {-1, -1};
}



void drawEdges(const struct graphPoint *p)
{
	if(p == NULL || p->numOfTargets == 0)
		return;

	struct coords start, end;
	start = getCoordsOfPoint(p);
	for(size_t i = 0; i < p->numOfTargets; i++)
	{
		end = getCoordsOfPoint(p->targets[i]);
		drawLine(start.x, start.y, end.x, end.y);
		cout 	<< "\tLine: " << endl
				<< "\t\t" << start.x << ":" << start.y << " -> " << end.x << ":" << end.y << endl;
	}
}




void drawObstacle(const struct obstacle *o)
{
	glSetColor(OBST_COLOR);
	drawQuad(o->c->x, o->c->y, o->a);
	glSetColor(GRAPH_COLOR);
	// for(int i = 0; i < 4; i++)
	// 	drawEdges(o->corners[i]);
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
		for(size_t i = 0; i < graphSize; i++)
		{
			drawEdges(graph[i]);
		}
		// glSetColor(0x00FF00);
		// drawLine(19, 40, 49, 70);
		// drawLine(startway.x, startway.y, endway.x, endway.y);


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
	numOfObstacles = 10;
	size_t iter = 0;
	obstacles = (struct obstacle*)malloc(sizeof(struct obstacle) * numOfObstacles);
	obstacles[iter++] = createObstacle(200, 200, 20);
	obstacles[iter++] = createObstacle(45, 55, 20);
	// obstacles[iter++] = createObstacle(70, 20, 20);
	// obstacles[iter++] = createObstacle(5, 55, 20);
	// obstacles[iter++] = createObstacle(5, 100, 20);
	// obstacles[iter++] = createObstacle(500, 55, 20);
	// obstacles[iter++] = createObstacle(300, 240, 20);
	// obstacles[iter++] = createObstacle(100, 90, 20);
	obstacles[iter++] = createObstacle(20, 280, 20);
	numOfObstacles = iter;

	graphSize = numOfObstacles * 4 + 2;

	struct graphPoint *p = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c;
	c.x = 200;
	c.y = 0;
	p->c = c;
	struct graphPoint *p2 = (struct graphPoint*)malloc(sizeof(struct graphPoint));

	struct coords c2;
	c2.x = 0;
	c2.y = 200;
	p2->c = c2;

	graph = (struct graphPoint**)malloc(graphSize * sizeof(struct graphPoint*));
	graph[numOfObstacles * 4] = p;
	graph[numOfObstacles * 4 + 1] = p2;
	for(size_t i = 0; i < numOfObstacles * 4; i++)
	{
		graph[i] = obstacles[i / 4].corners[i % 4];
	}

	startway.x 	= 0;
	startway.y 	= 0;
	endway.x	= 150;
	endway.y	= 150;


	// cout << "AAAAAAAAAAAAAAAA" << endl;


	// initObst(&obstacles[0]);
	// initPoint(graph[graphSize - 1], &c);
	initGraph(graph[numOfObstacles * 4], &c);

	// for(int i = 0; i < 4; i++)
	// {
	// 	if(obstacles[0].corners[i] != NULL)
	// 	{
	// 		for(int j = 0; j < obstacles[0].corners[i]->numOfTargets; j++)
	// 		{
	// 			cout << "corner: " << obstacles[0].corners[i]->c.x << ":" << obstacles[0].corners[i]->c.y << endl;
	// 		}
	// 	}
	// }

	// struct coords sss = getCoordsOfPoint(obstacles[0].corners[0]);
	// cout << sss.x << " " << sss.y << endl;
//
// 	struct vect v = createVect(&obstacles[0], CORNER_LEFT_BOT, -45);
// 	// v.dx = 5;
// 	// v.dy = 35;
// 	// struct vect v;
// 	// v.c = createCoords(19.4, 40.4);
// 	// v.dx =
// 	// cout << hasIntersection(&obstacles[1], &v) << endl;
//
// 	struct array ar;
// 	ar.items = obstacles;
// 	ar.size = numOfObstacles;
//
// 	struct coords cc = getIntersection(&obstacles[1], &v);
// 	cout << cc.x << ":" << cc.y << endl;
// 	//
// 	// struct obstacle *ob = getIntersected(&ar, &v);
// 	//
// 	// cout << ob << endl;
// 	// struct coords dot = {20, 10};
// 	// cout << isDotInside(&dot, &obstacles[0]);
// cout << "points: " << graphSize << "; calculated: " << calculatedPoints << endl;
// return 0;
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
