#pragma once
#include <Windows.h>
#include <math.h>

typedef struct tagVector
{
	float i;
	float j;
}Vector;

Vector Vector_GetFromPoints(POINT p1, POINT p2);
float Vector_DotProduct(Vector p1, Vector p2);
float Vector_GetMagnitude(Vector vector);
Vector Vector_Project(Vector vector, Vector onto);
Vector Vector_Add(Vector v1, Vector v2);
Vector Vector_AddRaw(Vector v1, float i, float j);
Vector Vector_Subtract(Vector v1, Vector v2);
POINT Vector_GetTerminalPoint(Vector v1, POINT offsetFromOrigin);
int Vector_RectContainsPoints(POINT rect[4], POINT point);