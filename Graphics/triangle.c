#include <stdint.h>
#include "graphics.h"
#include "triangle.h"


//============================================================================

//	PRIVATE PROTOTYPES

//============================================================================
static void sort_triangle_vertices(vec2_t* vertices);
static void fill_flat_bottom_triangle(vec2_t v0, vec2_t v1, vec2_t m, uint32_t color);
static void fill_flat_top_triangle(vec2_t v1, vec2_t m, vec2_t v2, uint32_t color);
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

	vec2_t v0 = triangle.points[0];
	vec2_t v1 = triangle.points[1];
	vec2_t v2 = triangle.points[2];
	vec2_t m;

	m.y = v1.y;

	//solve for mx using similar triangles (mx-x0 / x2 - x0) = ( y1-y0 / y2-y0 )
	m.x = ( ((v2.x - v0.x) * (v1.y - v0.y)) / (v2.y - v0.y) ) + v0.x;

	fill_flat_bottom_triangle(v0, v1, m, color);
	fill_flat_top_triangle(v1, m, v2, color);
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
static void fill_flat_bottom_triangle(vec2_t v0, vec2_t v1, vec2_t m, uint32_t color)
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

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;

	float changeInY = v1.y - v0.y;

	float slope1_increment = (float)(v1.x - v0.x) / changeInY;
	float slope2_increment = (float)(m.x - v0.x) / changeInY;

	float start_x = v0.x;
	float end_x = v0.x;

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
static void fill_flat_top_triangle(vec2_t v1, vec2_t m, vec2_t v2, uint32_t color)
{
	int y1 = (int)v1.y;
	int y2 = (int)v2.y;

	float changeInY = v2.y - v1.y;

	float slope1_increment = (float)(v1.x - v2.x) / changeInY;
	float slope2_increment = (float)(m.x - v2.x) / changeInY;

	float start_x = v2.x;
	float end_x = v2.x;

	for (int y = y2; y >= v1.y; y--)
	{
		draw_line((int)start_x, y, (int)end_x, y, color);
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
