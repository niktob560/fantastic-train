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

#define VECTORS_PER_CHECK	20

size_t QUAD_CHECK_SIDE = 5;

using namespace std;


bool terminated = false;
size_t numOfObstacles;


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
	ret.dy = ret.dx * tan(angle / 180 * PI);

	// cout <<  "\tcreate vect:" << endl
	// 	 << "\tdx: " << ret.dx << endl
	// 	 << "\tdy: " << ret.dy << endl
	// 	 << "\tx:  " << ret.c->x << endl
	// 	 << "\ty:  " << ret.c->y << endl;
	return ret;
}

struct array* getDataSet(const struct obstacle *obst, const uint8_t corner)
{
	struct graphPoint *p = getPoint(obst, corner);
	size_t size = 0;
	while (size < 2) {
		size = 0;
		for(size_t i = 0; i < numOfObstacles; i++)
		{
			struct coords *c = getCoordsOfCorner(obst, corner);
			if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE / 2
			&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE / 2)
				size++;
		}
		if(size < 2)
		{
			cout << "Increacing radius\n";
			QUAD_CHECK_SIDE += 40;
		}
	}
	cout << "Size: " << size << endl;
	size_t iter = 0;
	struct obstacle **obsts = (struct obstacle**)malloc(sizeof(struct obstacle*) * size);
	for(size_t i = 0; i < numOfObstacles; i++)
	{
		struct coords *c = getCoordsOfCorner(obst, corner);
		if(obstacles[i].c->x + obstacles[i].a >= c->x - QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->x <= c->x + obstacles[i].a + QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->y + obstacles[i].a >= c->y - QUAD_CHECK_SIDE / 2
		&& obstacles[i].c->y <= c->y + obstacles[i].a + QUAD_CHECK_SIDE / 2)
			obsts[iter++] = &(obstacles[i]);
	}
	struct array *ret = (struct array*)malloc(sizeof(struct array));
	ret->items = *obsts;
	ret->size = size;
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
	cout << "Watching for vect " << v->c->x << ":" << v->c->y << "; d" << v->dx << ":d" << v->dy << endl;
	for(size_t i = 0; i < dataset->size; i++)
	{
		cout << "###Watching for intersection, obst: " << (int)i << "\t\t###" << endl;
		inter = getIntersection(&((struct obstacle*)dataset->items)[i], v);
		COORDS_DATATYPE _l = getLen(v->c, &inter);
		if(inter.x != -1 && inter.y != -1 && _l < l)
		{
			cout << "####Found intersection: " << inter.x << ":" << inter.y << " with len " << _l << "\t\t\t####" << endl;
			o = &((struct obstacle*)dataset->items)[i];
			l = _l;
		}
	}
	return o;
}

void initObstCorner(const struct obstacle *obst, const uint8_t corner)
{
	cout << "Watching corner " << (int)corner << " of obstacle " << obst << endl;
	struct graphPoint *point = getPoint(obst, corner);
	struct array *obsts = getDataSet(obst, corner);
	struct vect v;
	for(uint8_t i = 0; i < VECTORS_PER_CHECK; i++)
	{
		// cout << "Creating vector " << (int)i << endl;
		v = createVect(obst, corner, ((360 / VECTORS_PER_CHECK) * i));
		// cout << "Watching vector " << (int)i << endl;
		struct obstacle *o = getIntersected(obsts, &v);
		if(o != NULL)
		{
			struct coords *c;
			for(uint8_t h = 0; h < 4; h++)
			{
				cout << "->Found intersection" << endl;
				c = getCoordsOfCorner(o, h);
				v.dx = abs(c->x - v.c->x);
				v.dy = abs(c->y - v.c->y);
				if((v.dx != 0 || v.dy != 0) && getIntersected(obsts, &v) == NULL)
				{
					cout << "--->Adding edge from o_" << obst->c->x << ":" << obst->c->y << " to o_" << o->c->x << ":" << o->c->y << "\t\t\t\t<---" << endl
						 << "With coords: " << v.c->x << ":" << v.c->y << " -> " << c->x << ":" << c->y << endl;
					addTarget(point, o->corners[h]);
				}
			}
		}
	}
	free(obsts);
}



void initObst(const struct obstacle *obst)
{
	for(uint8_t i = 0; i < 4; i++)
		initObstCorner(obst, i);
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
	for(int i = 0; i < 4; i++)
		drawEdges(o->corners[i]);
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
	numOfObstacles = 2;
	obstacles = (struct obstacle*)malloc(sizeof(struct obstacle) * numOfObstacles);
	obstacles[0] = createObstacle(20, 20, 20);
	obstacles[1] = createObstacle(50, 50, 20);
	// obstacles[2] = createObstacle(70, 20, 20);
	// obstacles[3] = createObstacle(5, 55, 20);

	startway.x 	= 0;
	startway.y 	= 0;
	endway.x	= 150;
	endway.y	= 150;



	// cout << "AAAAAAAAAAAAAAAA" << endl;

	initObst(&obstacles[0]);

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

	// struct vect v = createVect(&obstacles[0], CORNER_RIGHT_TOP, 45);
	// cout << hasIntersection(&obstacles[1], &v) << endl;

	// struct array ar;
	// ar.items = obstacles;
	// ar.size = 4;
	//
	// struct obstacle *ob = getIntersected(&ar, &v);
	//
	// cout << ob << endl;
	// struct coords dot = {20, 10};
	// cout << isDotInside(&dot, &obstacles[0]);
return 0;
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
