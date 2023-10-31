#include <stdint.h>
#include "graphics.h"
#include "triangle.h"


//============================================================================

//	PRIVATE PROTOTYPES

//============================================================================
static void sort_triangle_vertices(vec2_t* vertices);
static void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color);
static void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my, uint32_t color);
static void swap(vec2_t* v1, vec2_t* v2);
//============================================================================

/// <summary>
/// Fills out a triangle (Post Projection of vertices)
/// </summary>
/// <param name="triangle"></param>
void fill_triangle(triangle_t triangle, uint32_t color)
{
	//==================================================================================
	// Draw a filled triangle by cutting triangle in half, and drawing the top triangle
	// first, followed by drawing the bottom triangle 
	//
	//
	//	 			(x0, y0)
	//					/\
	//				   /| \
	//				  /	|  \
	//				 /  |   \
	//	  (x1,y1)  ========= (mX, mY) (CUT IN HALF)
	//				 \  |    \
	//				   \|	  \
	//				 	|\	   \
	//				 	|  \    \
	//				    |	 \   \
	//			     	|	  \   \
	//			     	|	    \  \
	//			     	|	      \ \
	//				 	|____________(x2, y2)
	// 
	//==================================================================================

	//sort by y coordinate so that v1 is the middle height
	sort_triangle_vertices(triangle.points);

	int x0 = (int) triangle.points[0].x;
	int y0 = (int) triangle.points[0].y;
	int x1 = (int) triangle.points[1].x;
	int y1 = (int) triangle.points[1].y;
	int x2 = (int) triangle.points[2].x;
	int y2 = (int) triangle.points[2].y;

	int my = y1; //same as midpoint since y1 is the middle height

	//solve for mx using similar triangles (mx-x0 / x2 - x0) = ( y1-y0 / y2-y0 )
	int mx = (int) (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

	if (y1 - y0 == 0)
	{
		fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
	}
	else if (y2 - y1 == 0)
	{
		fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
	}
	else 
	{
		fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
		fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
	}
}


/// <summary>
/// Sorts vertices from least to greatest by the y coordinate
/// </summary>
/// <param name="vertices"></param>
static void sort_triangle_vertices(vec2_t *vertices)
{
	// Swap vertices into ascending order v0.y < v1.y < v2.y
	vec2_t v0 = vertices[0];
	vec2_t v1 = vertices[1];
	vec2_t v2 = vertices[2];

	// a < b < c
	if (v0.y > v1.y) // sort first two elements
	{
		swap(&v0, &v1);
	}

	// b < a < c
	if (v1.y > v2.y) // sort last two elements
	{
		swap(&v1, &v2);
	}

	// b < c < a
	if (v0.y > v1.y) // bottom two (might need to be sorted again if the middle and end got pushed to the beginning)
	{
		swap(&v0, &v1);
	}

	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
}

/// <summary>
/// Fill flat bottom triangle
/// Starts from top going to bottom
/// </summary>
/// <param name="v0"></param>
/// <param name="v1"></param>
/// <param name="m"> => m.y = v1.y, (solve for m.x)</param>
static void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my, uint32_t color)
{
	//======================================
	//
	//	    	  (x0,y0)
	//				/|\
	//			   / | \
	//	 (slope1) /	 |  \ (slope2)
	//			 /   |	 \
	//			/	 |	  \
	//	(x1,y1) ----------- (xm, ym)
	//				 |
	//			  (x0,y1)
	//
	//======================================

	float changeInY = (float) y1 - y0;

	float slope1_increment = (x1 - x0) / (float) changeInY;
	float slope2_increment = (mx - x0) / (float) changeInY;

	float start_x = (float) x0;
	float end_x = (float) x0;

	for (int y = y0; y <= y1; y++)
	{
		draw_line((int) start_x, y, (int) end_x, y, color);
		start_x += slope1_increment; //calculate new startX
		end_x += slope2_increment; //calculate new endX
	}
}

/// <summary>
/// Fill flat top triangle
/// (Starts from bottom going up)
/// </summary>
/// <param name="v1"></param>
/// <param name="m"> => m.y = v1.y, (solve for m.x)</param>
/// <param name="v2"></param>
static void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color)
{
	float changeInY = (float) y2 - y1;

	float slope1_increment = (float) (x1 - x2) / changeInY;
	float slope2_increment = (float) (mx - x2) / changeInY;

	float start_x = (float) x2;
	float end_x = (float) x2;

	for (int y = y2; y >= y1; y--)
	{
		draw_line((int) start_x, y, (int) end_x, y, color);
		start_x += slope1_increment; //calculate new startX
		end_x += slope2_increment; //calculate new endX
	}
}

static void swap(vec2_t *v1, vec2_t *v2)
{
	vec2_t swap = *v1;

	*v1 = *v2;
	*v2 = swap;
}
