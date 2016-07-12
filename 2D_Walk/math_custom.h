#pragma once

#include <Windows.h>
#include <math.h>

#define M_PI 3.141592653589793238462643383279502884197169399375105820974

typedef struct tagVector
{
	int i;
	int j;
}Vector;

typedef struct tagSlope
{
	int dx;
	int dy;
}Slope;

int Greatest(int num, int num2);
int Least(int num, int num2);
int Difference(int num, int num2);
int Abs(int val);
double getRandomFloatInRange(double min, double max);
int GetColor(double* angle);
int GetRandRGB();
void specialPlot(int* screen, int screenWidth, int x, int y, int size, int color);

POINT* ToPoints(RECT rect);
int rectContainsPoint(POINT rect[4], POINT point);
Vector getVector(POINT p1, POINT p2);
int dotProduct(Vector p1, Vector p2);

Slope getSlope(POINT p1, POINT p2);
double getDistance(POINT p1, POINT p2);

int roundTo(int numToRound, int multiple);
int roundUpTo(int numToRound, int multiple);
int roundDownTo(int numToRound, int multiple);