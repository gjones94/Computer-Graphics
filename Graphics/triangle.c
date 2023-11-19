#include <stdint.h>
#include <stdio.h>
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

	vec2_t v0 = { .x = triangle.vertices[0].x,  .y = triangle.vertices[0].y };
	vec2_t v1 = { .x = triangle.vertices[1].x,  .y = triangle.vertices[1].y };
	vec2_t v2 = { .x = triangle.vertices[2].x,  .y = triangle.vertices[2].y };
	vec2_t m;

	//solve for mx using similar triangles (mx-x0 / x2 - x0) = ( y1-y0 / y2-y0 )
	m = get_opposite_midpoint(triangle);


	fill_flat_bottom_triangle(v0, v1, m, color);
	fill_flat_top_triangle(v1, m, v2, color);
}

void draw_texel(int x, int y, triangle_t triangle, uint32_t* texture)
{
	vec2_t p = { x, y };

	vec2_t a = vec2_from_vec4(triangle.vertices[0]);
	vec2_t b = vec2_from_vec4(triangle.vertices[1]);
	vec2_t c = vec2_from_vec4(triangle.vertices[2]);

	text2_t uv0 = triangle.texture_coordinates[0];
	text2_t uv1 = triangle.texture_coordinates[1];
	text2_t uv2 = triangle.texture_coordinates[2];

	// get weights between 0 and 1 for current point
	vec3_t weights = barycentric_weights(a, b, c, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	// get interpolated uv based on current point
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	float w0 = triangle.vertices[0].w;
	float w1 = triangle.vertices[1].w;
	float w2 = triangle.vertices[2].w;

	// Perform interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = ((uv0.u / w0) * alpha) + ((uv1.u / w1) * beta) + ((uv2.u / w2) * gamma);
	interpolated_v = ((uv0.v / w0) * alpha) + ((uv1.v / w1) * beta) + ((uv2.v / w2) * gamma);

	interpolated_reciprocal_w = ((1/w0) * alpha) + ((1/w1) * beta) + ((1/w2) * gamma);

	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	// Scale weighted uv according to png size
	int text_x = abs((int)(interpolated_u * texture_width)) % texture_width;
	int text_y = abs((int)(interpolated_v * texture_height)) % texture_height;

	// Get flat array index based on x and y
	//                              ROW            COLUMN
	int texture_index = ((texture_width * text_y) + text_x);

	float previousZBuffer = z_buffer[(WINDOW_WIDTH * y) + x];

	/* 
	* as z decreases, the reciprocal gets larger since we have 1 / z
	* so, we subtract from 1 to get the small value lesser(nearer) z value
	*/
	
	interpolated_reciprocal_w = 1 - interpolated_reciprocal_w;
	if (interpolated_reciprocal_w < previousZBuffer) // Only update if depth is nearer than previous
	{
		draw_pixel(x, y, texture[texture_index]);
		z_buffer[(WINDOW_WIDTH * y) + x] = interpolated_reciprocal_w;
	}
}
	

void fill_textured_triangle(triangle_t triangle, uint32_t* texture)
{
	sort_triangle_vertices(&triangle);

	vec4_t v0 = triangle.vertices[0];
	vec4_t v1 = triangle.vertices[1];
	vec4_t v2 = triangle.vertices[2];

	// Fill Top Triangle (Flat Bottom)
	int y0 = (int) v0.y;
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;

	float slope1 = 0;
	float slope2 = 0;

	// Prevent division by 0
	if (y1 - y0 != 0)
	{
		slope1 = (float) ((int) v1.x - (int) v0.x) / (float) (y1 - y0);
	}

	// Prevent division by 0
	if (y2 - y0 != 0)
	{
		slope2 = (float) ((int) v2.x - (int) v0.x) / (float) (y2 - y0);
	}

	// Make sure top triangle with flat bottom actually exists
	if (y1 - y0 != 0)
	{
		float x1 = v0.x;
		float x2 = v0.x;

		for (int y = y0; y <= y1; y++)
		{
			//draw_line((int) start_x, y, (int) end_x, y, 0xFF0000FF);
			float start_x = x1 < x2 ? x1 : x2;
			float end_x = x1 < x2 ? x2 : x1;

			for (int x = (int) start_x; x <= (int) end_x; x++)
			{
				draw_texel(x, y, triangle, texture);
				//draw_pixel(x, y, 0xFF0000FF);
				//printf("X: %d at height %d\n", (int) x, (int) y);
			}

			x1 += slope1; //calculate new startX
			x2 += slope2; //calculate new endX
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

	// Make sure the bottom triangle actually exists
	if (y2 - y1 != 0)
	{
		float x1 = v2.x;
		float x2 = v2.x;

		for (int y = y2; y >= y1; y--)
		{
			// Obtain the starting x value
			float start_x = x1 < x2 ? x1 : x2;

			// Obtain the ending x value
			float end_x = x1 < x2 ? x2: x1;

			for (int x = (int) start_x; x <= (int) end_x; x++)
			{
				draw_texel(x, y, triangle, texture);
			}

			x1 += slope1;
			x2 += slope2;
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
	vec4_t *v0 = &triangle->vertices[0];
	vec4_t *v1 = &triangle->vertices[1];
	vec4_t *v2 = &triangle->vertices[2];

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

static void swap_vectors(vec4_t *v1, vec4_t *v2)
{
	vec4_t swap = *v1;

	*v1 = *v2;
	*v2 = swap;
}


static void swap_triangles(triangle_t* triangle1, triangle_t* triangle2)
{
	triangle_t swap = *triangle2;
	*triangle2 = *triangle1;
	*triangle1 = swap;
}
