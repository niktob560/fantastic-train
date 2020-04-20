#pragma once

#include <GL/glut.h>
#include <string>
#include <FTGL/ftgl.h>

#include "math.hpp"

void glSetColor(uint32_t color);

void drawLine(float x1, float y1, float x2, float y2);

void drawRect(float x, float y, float a, float b);

void drawRect(float x, float y, float a, float b, float rot);

void drawTriangle(float x, float y, float a);

void drawCircle(float cx, float cy, float r, int num_segments);

void drawCircleFilled(float cx, float cy, float r, int numSegments);

void drawOct(float x, float y, float a);

void drawQuad(float x, float y, float a);

void drawText(std::wstring text, unsigned int size, int x, int y);
