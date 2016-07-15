#include "math_custom.h"

int Greatest(int num, int num2)
{
	if (num > num2) return num;
	return num2;
}

int Least(int num, int num2)
{
	if (num < num2) return num;
	return num2;
}

int Difference(int num, int num2)
{
	return Greatest(num, num2) - Least(num, num2);
}

int Abs(int val)
{
	if (val < 0) return -val;
	return val;
}

/*
Get Random Float In Range

name: getRandomFloatInRange

desc: This function will generate a random floating point number between
the min and max

params: (1) int min - The minimum size of the floating point number
(2) int max - The maximum size of the floating point number

returns: An integer representing the number of tries allowed to the user
*/
double getRandomFloatInRange(double min, double max)
{
	return (((double)rand() / RAND_MAX)*(max - min)) + min;
}

int GetColor(double* angle)
{
	*angle += 0.05;
	if (*angle > 2 * M_PI)
		*angle = -(2 * M_PI);
	return RGB(sin(*angle + 0) * 127 + 128, sin(*angle + 2) * 127 + 128, sin(*angle + 4) * 127 + 128);
}

int GetRandRGB()
{
	int color = 0;
	while (!color)
		color = RGB(rand() % 256, rand() % 256, rand() % 256);
	return color;
}

//"special plot" = square plot
void specialPlot(int* screen, int screenWidth, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int z = 0; z < size; z++)
		{
			screen[((y + z)*screenWidth) + x + i] = color;
		}
	}
}

POINT* convertRectToPoints(RECT rect)
{
	POINT* points = malloc(4 * sizeof(POINT));

	int height = Difference(rect.top, rect.bottom);
	int width = Difference(rect.left, rect.right);

	points[2].x = rect.left;
	points[2].y = height;

	points[3].x = width;
	points[3].y = height;

	points[0].x = rect.left;
	points[0].y = Least(rect.top, rect.bottom);

	points[1].x = width;
	points[1].y = Least(rect.top, rect.bottom);

	return points;
}

Slope getSlope(POINT p1, POINT p2)
{
	Slope slope = { 0 };

	if (p2.x - p1.x > 0)
		slope.dx = 1;
	else if (p1.x - p2.x > 0)
		slope.dx = -1;
	else if (p2.x - p1.x == 0)
		slope.dx = 0;

	if (p2.y - p1.y > 0)
		slope.dy = 1;
	else if (p1.y - p2.y > 0)
		slope.dy = -1;
	else if (p2.y - p1.y == 0)
		slope.dy = 0;

	return slope;
}

double getDistance(POINT p1, POINT p2)
{
	return sqrt(((p2.x - p1.x)*(p2.x - p1.x)) + ((p2.y - p1.y)*(p2.y - p1.y)));
}

//most credit -> http://stackoverflow.com/a/3407254/2396111
int roundTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (remainder < multiple / 2 + 5)
	{
		return numToRound - remainder;
	}

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

//all credit -> http://stackoverflow.com/a/3407254/2396111
int roundUpTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

//most credit -> http://stackoverflow.com/a/3407254/2396111
int roundDownTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) + remainder);
	else
		return numToRound - remainder;
}