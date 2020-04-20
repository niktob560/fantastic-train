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

#include "routines.hpp"
#include "fantastic-train.hpp"

#define GL_TIMER_DELAY	400
#define RETURN_CODE_OK 0
#define OBST_COLOR 	0x268BD2
#define GRAPH_COLOR 0x99D4AA
#define DATA_COLOR	0xD9E6F2
#define WAY_COLOR	0xAA99D4
#define BG_COLOR	0x1C2639
#define DATA_EXTRA_COLOR 0xDC322F


using namespace std;

using graphbases::coords;

bool terminated = false;






void drawLine(struct coords *start, struct coords *end)
{
	drawLine(start->x, start->y, end->x, end->y);
}

// struct obstacle* getRootObstacle(struct graphPoint *p)
// {
// 	for(size_t i = 0; i < fantastictrain::getNumOfObstacles(); i++)
// 	{
// 		for(size_t j = 0; j < 4; j++)
// 		{
// 			if(obstacles[i].corners[j]->i == p->i)
// 				return &obstacles[i];
// 		}
// 	}
// 	return NULL;
// }


void drawLine(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end)
{
	struct coords 	c1 = geometry::getCoordsOfPoint(start), 
					c2 = geometry::getCoordsOfPoint(end);
	drawLine(&c1, &c2);
}





void drawEdges(struct graphbases::graphPoint *p)
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




void drawObstacle(const struct bases::obstacle *o)
{
	glSetColor(OBST_COLOR);
	drawRect(o->c.x, o->c.y, o->a, o->b, static_cast<float>((o->rot * M_PI) / 90.0));
}


void drawWay(size_t pointNum, size_t target)
{
	size_t curr = pointNum;
	while(curr != target)
	{
		glSetColor(WAY_COLOR);
		drawLine(&bases::graph[curr], &bases::graph[fantastictrain::getWaysVector()[curr]]);
		curr = fantastictrain::getWaysVector()[curr];
	}
}

void drawDots()
{
	for(size_t i = 0; i < fantastictrain::getGraphSize(); i++)
	{
		glSetColor(DATA_EXTRA_COLOR);
		drawCircleFilled(geometry::getCoordsOfPoint(&bases::graph[i]).x, geometry::getCoordsOfPoint(&bases::graph[i]).y, 3, 10);
		glSetColor(DATA_COLOR);
		std::wstring str = L"";
		for(size_t k = 0; k < fantastictrain::getNumOfObstacles(); k++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(bases::obstacles[k].corners[j]->i == bases::graph[i].i)
				{
					str = to_wstring(j);
					k = fantastictrain::getNumOfObstacles();
					break;
				}
			}
		}
		drawText((str), 12, geometry::getCoordsOfPoint(&bases::graph[i]).x, geometry::getCoordsOfPoint(&bases::graph[i]).y);
	}
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
		glSetColor(BG_COLOR);
		drawRect(static_cast<float>(glutGet(GLUT_SCREEN_WIDTH) / 2.0), static_cast<float>(glutGet(GLUT_SCREEN_HEIGHT) / 2.0), static_cast<float>(glutGet(GLUT_SCREEN_WIDTH)), static_cast<float>(glutGet(GLUT_SCREEN_HEIGHT)));
		for(size_t i = 0; i < fantastictrain::getNumOfObstacles(); i++)
		{
			drawObstacle(&bases::obstacles[i]);
			glSetColor(DATA_COLOR);
			drawText(to_wstring(i), 12, bases::obstacles[i].c.x, bases::obstacles[i].c.y);
		}
		for(size_t i = 0; i < fantastictrain::getGraphSize(); i++)
		{
			glLineWidth(1);
			drawEdges(&bases::graph[i]);
		}

		glLineWidth(5);
		glSetColor(WAY_COLOR);
		drawWay(1, 0);

		glSetColor(DATA_EXTRA_COLOR);
		drawText(to_wstring(static_cast<unsigned int>(bases::graph[1].weight)), 14, 10, 10);

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
	fantastictrain::init();

	fantastictrain::addObstacle({-5, 1020 / 2}, 	10, 	1020, 	0);
	fantastictrain::addObstacle({1020, 1020 / 2}, 	10, 	1020, 	0);
	fantastictrain::addObstacle({1020 / 2, -5}, 	1020, 	10, 	0);
	fantastictrain::addObstacle({1020 / 2, 1020}, 	1020, 	10, 	0);

	fantastictrain::addObstacle({100, 100}, 100, 100, 34);
	fantastictrain::addObstacle({310, 350}, 200, 300, 20);
	fantastictrain::addObstacle({280, 580}, 20 , 20 , 40);
	fantastictrain::addObstacle({80	, 330}, 50 , 40 , 60);
	fantastictrain::addObstacle({120, 340}, 50 , 49 , 120);
	fantastictrain::addObstacle({60	, 400}, 30 , 32 , 250);
	fantastictrain::addObstacle({710, 500}, 200, 100, 34);
	fantastictrain::addObstacle({910, 400}, 200, 100, 80);
	fantastictrain::addObstacle({920, 500}, 200, 100, 70);
	fantastictrain::addObstacle({910, 100}, 150, 100, 60);
	fantastictrain::addObstacle({110, 500}, 200, 100, 50);
	fantastictrain::addObstacle({10	, 600}, 200, 100, 40);
	fantastictrain::addObstacle({500, 400}, 200, 100, 40);
	fantastictrain::addObstacle({650, 300}, 200, 100, 30);
	fantastictrain::addObstacle({350, 700}, 200, 100, 20);
	fantastictrain::addObstacle({150, 600}, 200, 100, 10);
	fantastictrain::addObstacle({550, 550}, 200, 100, 10);
	fantastictrain::addObstacle({750, 550}, 200, 100, 120);

	fantastictrain::setTargetCoords({500, 1000});
	fantastictrain::setHomeCoords({30, 30});

	fantastictrain::run();


	cout << endl;
	for(size_t i = 0; i < fantastictrain::getGraphSize(); i++)
		cout << setw(3) << i;
	cout << endl;
	for(size_t i = 0; i < fantastictrain::getGraphSize(); i++)
		cout << setw(3) << static_cast<int>(fantastictrain::getWaysVector()[i]);
	cout << endl;



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
