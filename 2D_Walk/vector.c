#include "vector.h"

Vector Vector_GetFromPoints(FloatPoint p1, FloatPoint p2)
{
	Vector toReturn = { 0 };
	toReturn.i = p2.x - p1.x;
	toReturn.j = p2.y - p1.y;
	return toReturn;
}

float Vector_DotProduct(Vector p1, Vector p2)
{
	return ((p1.i*p2.i) + (p1.j*p2.j));
}

float Vector_GetMagnitude(Vector vector)
{
	return (float)sqrt(vector.i*vector.i + vector.j*vector.j);
}

Vector Vector_Project(Vector vector, Vector onto)
{
	float dp = Vector_DotProduct(vector, onto);
	float magnitude = Vector_GetMagnitude(onto);
	float magsquared = magnitude*magnitude;;
	float scalar = dp / magsquared;

	Vector projection = { 0 };
	projection.i = vector.i*scalar;
	projection.j = vector.j*scalar;
	return projection;
}

Vector Vector_Add(Vector v1, Vector v2)
{
	Vector vector;
	vector.i = v1.i + v2.i;
	vector.j = v1.j + v2.j;
	return vector;
}

Vector Vector_AddRaw(Vector v1, float i, float j)
{
	Vector vector;
	vector.i = v1.i + i;
	vector.j = v1.j + j;
	return vector;
}

//please note that it's v1 - v2.
Vector Vector_Subtract(Vector v1, Vector v2)
{
	Vector vector;
	vector.i = v1.i - v2.i;
	vector.j = v1.j - v2.j;
	return vector;
}

POINT Vector_GetTerminalPoint(Vector v1, POINT offsetFromOrigin)
{
	POINT point;
	point.x = v1.i + offsetFromOrigin.x;
	point.y = v1.j + offsetFromOrigin.y;
	return point;
}

//uses vectors for fancy stuff :)
//formula: (0<AM[dot]AB<AB[dot]AB)/\(0<AM[dot]AD<AD[dot]AD) where M is a point an A,B,D are top left, top right, and bottom right respectively. [dot] represents the dot as in dot product.
int Vector_RectContainsPoints(FloatPoint rect[4], FloatPoint point)
{
	Vector AM = Vector_GetFromPoints(rect[0], point);
	Vector AB = Vector_GetFromPoints(rect[0], rect[1]);
	Vector BC = Vector_GetFromPoints(rect[1], rect[2]);
	Vector BM = Vector_GetFromPoints(rect[1], point);

	float dotAMAB = Vector_DotProduct(AM, AB);
	float dotABAB = Vector_DotProduct(AB, AB);
	float dotBCBM = Vector_DotProduct(BC, BM);
	float dotBCBC = Vector_DotProduct(BC, BC);

	if (((0 <= dotAMAB) && (dotAMAB <= dotABAB)) && ((0 <= dotBCBM) && (dotBCBM <= dotBCBC)))
		return 1;
	else
		return 0;
}