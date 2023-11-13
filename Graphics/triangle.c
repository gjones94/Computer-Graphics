#include <stdint.h>
#include "graphics.h"
#include "triangle.h"
#include <math.h>


//============================================================================
//
//	PRIVATE PROTOTYPES
//
//============================================================================
static void sort_triangle_vertices(triangle_t* triangle);
static void fill_flat_bottom_triangle(vec2_t v0, vec2_t v1, vec2_t m, uint32_t color);
static void fill_flat_top_triangle(vec2_t v1, vec2_t m, vec2_t v2, uint32_t color);
static vec2_t get_opposite_midpoint(triangle_t triangle);

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
    //               (x0, y0)
    //                  /\
    //                 /| \
    //                / |  \
    //               /  |   \
    //       (x1,y1) ========= (mX, mY) (CUT IN HALF, must solve for midpoint m.x)
    //               \  |    \
    //                 \|     \
    //                  |\     \
    //                  |  \    \
    //                  |    \   \
    //                  |     \   \
    //                  |       \  \
    //                  |         \ \
    //                  |____________(x2, y2)
    // 
    //==================================================================================

	//sort by y coordinate so that v1 is the middle height
	sort_triangle_vertices(&triangle);

	vec2_t v0 = triangle.vertices[0];
	vec2_t v1 = triangle.vertices[1];
	vec2_t v2 = triangle.vertices[2];
	vec2_t m;

	//solve for mx using similar triangles (mx-x0 / x2 - x0) = ( y1-y0 / y2-y0 )
	m = get_opposite_midpoint(triangle);


	fill_flat_bottom_triangle(v0, v1, m, color);
	fill_flat_top_triangle(v1, m, v2, color);
}

void fill_textured_triangle(triangle_t triangle, uint32_t* texture)
{
	sort_triangle_vertices(&triangle);

	vec2_t v0 = triangle.vertices[0];
	vec2_t v1 = triangle.vertices[1];
	vec2_t v2 = triangle.vertices[2];

	// Fill Top Triangle (Flat Bottom)

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;

	float slope1 = 0;
	float slope2 = 0;

	if (y1 - y0 != 0)
	{
		slope1 = (v1.x - v0.x) / (float) (y1 - y0);
	}

	if (y2 - y0 != 0)
	{
		slope2 = (v2.x - v0.x) / (float) (y2 - y0);
	}

	float start_x = v0.x;
	float end_x = v0.x;

	if (y1 - y0 != 0)
	{
		for (int y = y0; y <= y1; y++)
		{
			draw_line((int)start_x, y, (int)end_x, y, 0xFF0000FF);

			end_x += slope1; //calculate new startX
			start_x += slope2; //calculate new endX

			/*if (start_x > end_x)
			{
				float temp = start_x;
				start_x = end_x;
				end_x = temp;
			}*/

			/*for (int x = (int)start_x; x < (int)end_x; x++)
			{
				draw_pixel(x, y, 0xFF0000FF);
			}*/
		}
	}

	// Fill Bottom Triangle (Flat Top)
	slope1 = 0;
	slope2 = 0;

	if (y2 - y1 != 0)
	{
		slope1 = (v1.x - v2.x) / (float) (y2 - y1);
	}

	if (y2 - y0 != 0)
	{
		slope2 = (v0.x - v2.x) / (float) (y2 - y0);
	}

	if (y2 - y1 != 0)
	{
		start_x = v2.x;
		end_x = v2.x;

		for (int y = y2; y >= y1; y--)
		{
			draw_line((int) start_x, y, (int) end_x, y, 0xFF0000FF);

			/*
			for (int x = (int)start_x; x < (int)end_x; x++)
			{
				draw_pixel(x, y, 0xFF0000FF);
			}
			*/

			start_x += slope1;
			end_x += slope2;

			/*if (start_x > end_x)
			{
				float swap = start_x;
				start_x = end_x;
				end_x = swap;
			}*/
		}
	}
}

void sort_by_depth(triangle_t* triangles, int num_triangles)
{
	for (int i = 0; i < num_triangles; i++)
	{
		for (int j = 0; j < num_triangles; j++)
		{
			if (triangles[i].avg_depth > triangles[j].avg_depth)
			{
				swap_triangles(&triangles[i], &triangles[j]);
			}
		}

	}
}

/// <summary>
/// Sorts vertices from least to greatest by the y coordinate
/// </summary>
/// <param name="vertices"></param>
static void sort_triangle_vertices(triangle_t *triangle)
{
	// Swap vertices into ascending order v0.y < v1.y < v2.y
	vec2_t *v0 = &triangle->vertices[0];
	vec2_t *v1 = &triangle->vertices[1];
	vec2_t *v2 = &triangle->vertices[2];

	text2_t *uv0 = &triangle->texture_coordinates[0];
	text2_t *uv1 = &triangle->texture_coordinates[1];
	text2_t *uv2 = &triangle->texture_coordinates[2];

	// a < b < c
	if (v0->y > v1->y) // sort first two elements
	{
		swap_vectors(v0, v1);
		swap_textures(uv0, uv1);
	}

	// b < a < c
	if (v1->y > v2->y) // sort last two elements
	{
		swap_vectors(v1, v2);
		swap_textures(uv1, uv2);
	}

	// b < c < a
	if (v0->y > v1->y) // bottom two (might need to be sorted again if the middle and end got pushed to the beginning)
	{
		swap_vectors(v0, v1);
		swap_textures(uv0, uv1);
	}
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
    //            (x0,y0)
    //              /|\
    //             / | \
    //   (slope1) /  |  \ (slope2)
    //           /   |   \
    //          /    |    \
    //  (x1,y1) ----------- (xm, ym)
    //               |
    //            (x0,y1)
    //
    //======================================

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;

	// If the change was less than 1, division caused asymptotic large numbers
	// so we need to keep the result as a whole number
	int changeInY = y1 - y0;

	if (changeInY == 0) 
	{
		// There is no top triangle, midpoint and top point are at same height
		return;
	}

	float slope1_increment = (v1.x - v0.x) / (float) changeInY;
	float slope2_increment = (m.x - v0.x) / (float) changeInY;

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
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;

	// If the change was less than 1, division caused asymptotic large numbers
	// so we need to keep the result as a whole number
	int changeInY = y2 - y1;

	if (changeInY == 0)
	{
		// There is no bottom triangle, midpoint and bottom point are at same height
		return;
	}

	float slope1_increment = (v1.x - v2.x) / (float) changeInY;
	float slope2_increment = (m.x - v2.x) / (float) changeInY;

	float start_x = v2.x;
	float end_x = v2.x;

	for (int y = y2; y >= y1; y--)
	{
		draw_line((int) start_x, y, (int) end_x, y, color);
		start_x += slope1_increment;
		end_x += slope2_increment; 
	}
}
static vec2_t get_opposite_midpoint(triangle_t triangle)
{
	vec2_t midpoint;

	float x0 = triangle.vertices[0].x;
	float y0 = triangle.vertices[0].y;
	float x1 = triangle.vertices[1].x;
	float y1 = triangle.vertices[1].y;
	float x2 = triangle.vertices[2].x;
	float y2 = triangle.vertices[2].y;

    //==================================================================================
    //
    //
    //               (x0, y0)
    //                  /\
    //                 /| \
    //                / |  \
    //               /  |   \
    //       (x1,y1) ========= (mX, mY) (CUT IN HALF, must solve for midpoint m.x)
    //               \  |    \
    //                 \|     \
    //                  |\     \
    //                  |  \    \
    //                  |    \   \
    //                  |     \   \
    //                  |       \  \
    //                  |         \ \
    //                  |____________(x2, y2)
    // 
    //==================================================================================

	midpoint.y = triangle.vertices[1].y; // the triangle points were sorted so that y1 is in the middle of y0 and y2
	/*
		Solve for mx using similar triangles (mx-x0 / x2 - x0) = ( y1-y0 / y2-y0 )
	*/
	midpoint.x = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

	return midpoint;
}

static void swap_vectors(vec2_t *v1, vec2_t *v2)
{
	vec2_t swap = *v1;

	*v1 = *v2;
	*v2 = swap;
}


static void swap_triangles(triangle_t* triangle1, triangle_t* triangle2)
{
	triangle_t swap = *triangle2;
	*triangle2 = *triangle1;
	*triangle1 = swap;
}
