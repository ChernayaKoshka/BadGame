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

Vector getVector(POINT p1, POINT p2)
{
	Vector toReturn = { 0 };
	toReturn.i = p2.x - p1.x;
	toReturn.j = p2.y - p1.y;
	return toReturn;
}

int dotProduct(Vector p1, Vector p2)
{
	return ((p1.i*p2.i) + (p1.j*p2.j));
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

//uses vectors for fancy stuff :)
//formula: (0<AM[dot]AB<AB[dot]AB)/\(0<AM[dot]AD<AD[dot]AD) where M is a point an A,B,D are top left, top right, and bottom right respectively. [dot] represents the dot as in dot product.
int rectContainsPoint(POINT rect[4], POINT point)
{
	Vector AM = getVector(rect[0], point);
	Vector AB = getVector(rect[0], rect[1]);
	Vector BC = getVector(rect[1], rect[2]);
	Vector BM = getVector(rect[1], point);

	int dotAMAB = dotProduct(AM, AB);
	int dotABAB = dotProduct(AB, AB);
	int dotBCBM = dotProduct(BC, BM);
	int dotBCBC = dotProduct(BC, BC);

	if (((0 <= dotAMAB) && (dotAMAB <= dotABAB)) && ((0 <= dotBCBM) && (dotBCBM <= dotBCBC)))
		return 1;
	else
		return 0;
}

BOOL onSegment(POINT p, POINT q, POINT r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return TRUE;
	return FALSE;
}

int findOrientation(POINT p, POINT q, POINT r)
{
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
BOOL doLinesIntersect(POINT p1, POINT q1, POINT p2, POINT q2)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = findOrientation(p1, q1, p2);
	int o2 = findOrientation(p1, q1, q2);
	int o3 = findOrientation(p2, q2, p1);
	int o4 = findOrientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return TRUE;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return TRUE;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return TRUE;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return TRUE;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return TRUE;

	return FALSE;
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