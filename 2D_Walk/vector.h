#pragma once
#include <Windows.h>
#include <math.h>

typedef struct tagVector
{
	float i;
	float j;
}Vector;

typedef struct tagFloatPoint
{
	float x;
	float y;
}FloatPoint;

Vector Vector_GetFromPoints(FloatPoint p1, FloatPoint p2);
float Vector_DotProduct(Vector p1, Vector p2);
float Vector_GetMagnitude(Vector vector);
Vector Vector_Project(Vector vector, Vector onto);
Vector Vector_Add(Vector v1, Vector v2);
Vector Vector_AddRaw(Vector v1, float i, float j);
Vector Vector_Subtract(Vector v1, Vector v2);
POINT Vector_GetTerminalPoint(Vector v1, POINT offsetFromOrigin);
int Vector_RectContainsPoints(FloatPoint rect[4], FloatPoint point);