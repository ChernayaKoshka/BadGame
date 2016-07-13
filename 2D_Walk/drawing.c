#include "math_custom.h"
#include "drawing.h"

void Plot(int x, int y, int color, int* buffer, int bufferWidth)
{
	buffer[(y*bufferWidth) + x] = color;
}

//code credit of http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
void DrawLine(int x, int y, int x2, int y2, int color, int* buffer, int bufferWidth)
{
	//width
	int w = x2 - x;
	//height
	int h = y2 - y;
	//initialize "deltax/y", delta means change
	//so dx1 is "change in x1"
	int dx1 = 0;
	int dy1 = 0;
	int dx2 = 0;
	int dy2 = 0;
	//is our width negative or postive?
	//set deltax1 accordingly
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	//ditto
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	//same as before...
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	//get |width|
	int longest = Abs(w);
	//get |shortest|
	int shortest = Abs(h);
	//another octant check, is our width less than (or equal to) our height?
	if (!(longest > shortest))
	{
		//swap width/height
		longest = Abs(h);
		shortest = Abs(w);
		//another octant check! Is our height negative? Set deltay accordingly
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		//no change in x2
		dx2 = 0;
	}
	int numerator = longest >> 1; //faster way to divide by two
								  //go along the length of the triangle and...
	for (int i = 0; i <= longest; i++)
	{
		//plot the point at x/y
		Plot(x, y, color, buffer, bufferWidth);
		//increase the size of the numerator by our "shortest" (h/w, whichever is smaller)
		numerator += shortest;

		//if the numerator is greater than or equal to our "longest" (h/w, whichever is bigger)
		if (!(numerator < longest))
		{
			//decrease the size of the numerator by our "longest"
			numerator -= longest;
			//increase x by our CHANGE IN X
			x += dx1;
			//ditto but with y
			y += dy1;
		}
		else
		{
			//increase our x by our CHANGE IN X2
			x += dx2;
			//ditto but with y
			y += dy2;
		}
	}
}

void DrawRect(int x, int y, int width, int height, int color, int* buffer, int bufferWidth)
{
	for (int i = 0; i < height; i++)
	{
		DrawLine(x, (y + i), width, (y + i), color, buffer, bufferWidth);
	}
}

void DrawTriangle(TRIANGLE tri, int color, int* buffer, int bufferWidth)
{
	/*
		1
	  /   \
	 /	   \
	/	    \
	2-------3

	1 = tri.top
	2 = tri.left
	3 = tri.right
	*/

	//2->1
	DrawLine(tri.left.x, tri.left.y, tri.top.x, tri.top.y, color, buffer, bufferWidth);
	//2->3
	DrawLine(tri.left.x, tri.left.y, tri.right.x, tri.right.y, color, buffer, bufferWidth);
	//3->1
	DrawLine(tri.right.x, tri.right.y, tri.top.x, tri.top.y, color, buffer, bufferWidth);
}

void DrawCircle(int x, int y, int radius, int color, int* buffer, int bufferWidth)
{
	int x1 = radius;
	int y1 = 0;
	int decisionOver2 = 1 - x1;

	while (y1 <= x1)
	{
		Plot(x1 + x, y1 + y, color, buffer, bufferWidth); // Octant 1
		Plot(y1 + x, x1 + y, color, buffer, bufferWidth); // Octant 2
		Plot(-x1 + x, y1 + y, color, buffer, bufferWidth); // Octant 4
		Plot(-y1 + x, x1 + y, color, buffer, bufferWidth); // Octant 3
		Plot(-x1 + x, -y1 + y, color, buffer, bufferWidth); // Octant 5
		Plot(-y1 + x, -x1 + y, color, buffer, bufferWidth); // Octant 6
		Plot(x1 + x, -y1 + y, color, buffer, bufferWidth); // Octant 7
		Plot(y1 + x, -x1 + y, color, buffer, bufferWidth); // Octant 8
		y1++;
		if (decisionOver2 <= 0)
		{
			decisionOver2 += 2 * y1 + 1;
		}
		else
		{
			x1--;
			decisionOver2 += 2 * (y1 - x1) + 1;
		}
	}
}