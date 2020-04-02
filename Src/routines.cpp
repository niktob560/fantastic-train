#include "routines.hpp"


void glSetColor(uint32_t color)
{
	glColor3f((float)((color >> 16) & 0xFF) / 255, (float)((color >> 8) & 0xFF) / 255, (float)(color & 0xFF) / 255);
}


void drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
	
    drawCircleFilled(x2, y2, 1, 10);
    drawCircleFilled(x1, y1, 1, 10);
}

void drawRect(float x, float y, float a, float b)
{
	x -= a / 2;
	y -= b / 2;
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d(x, y + b);
	glVertex2d(x + a, y + b);
	glVertex2d(x + a, y);
	glEnd();
}

void drawTriangle(float x, float y, float a)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x, y);
	glVertex2d(x + a, y);
	glVertex2d(x + (a / 2), y + a);
	glEnd();
}

void drawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

void drawCircleFilled(float cx, float cy, float r, int numSegments)
{
	glBegin(GL_POLYGON);
    for(int ii = 0; ii < numSegments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(numSegments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

void drawOct(float x, float y, float a)
{
	float alpha = 22.35;

	glBegin(GL_POLYGON);
	glVertex2d(x + a * sin(alpha), y + a * cos(alpha));
	glVertex2d(x - a * sin(alpha), y + a * cos(alpha));
	glVertex2d(x - a * cos(alpha), y + a * sin(alpha));
	glVertex2d(x - a * cos(alpha), y - a * sin(alpha));
	glVertex2d(x - a * sin(alpha), y - a * cos(alpha));
	glVertex2d(x + a * sin(alpha), y - a * cos(alpha));
	glVertex2d(x + a * cos(alpha), y - a * sin(alpha));
	glVertex2d(x + a * cos(alpha), y + a * sin(alpha));
	glEnd();
}

void drawQuad(float x, float y, float a)
{
	drawRect(x, y, a, a);
}


void drawText(std::wstring text, int size, int x, int y)
{
	FTGLPixmapFont hack_italic_font("/usr/share/fonts/TTF/JetBrainsMonoNL-Regular.ttf");
	if(!hack_italic_font.Error())
	{
		glPushMatrix();
		//const wchar_t * tarr = (wchar_t*)malloc(text.length());
		//wstrcpy(tarr, text);
		hack_italic_font.FaceSize(size);
		glRasterPos2f(x, y);//TODO: refactor
		hack_italic_font.Render(text.c_str());
		glPopMatrix();
	}
	else
	{
		// std::cout << "FONT ERR\n";
	}
}
