#include <stdlib.h>
#include "mesh.h"
#include "graphics.h"
#include "array.h"

mesh_t mesh =
{
	.vertices = NULL,
	.faces = NULL,
	.rotation = { .x = 0, .y = 0, .z = 0 }
};

// Constant vertices for a cube
vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ .x = -1, .y = -1, .z = -1},	//0
	{ .x = -1, .y =  1, .z = -1},	//1
	{ .x =  1, .y =  1, .z = -1},	//2
	{ .x =  1, .y = -1, .z = -1},	//3
	{ .x =  1, .y =  1, .z =  1},	//4
	{ .x =  1, .y = -1, .z =  1},	//5
	{ .x = -1, .y =  1, .z =  1},	//6
	{ .x = -1, .y = -1, .z =  1},	//7
};

// constant faces for a cube
face_t cube_faces[N_CUBE_FACES] = {
	//front
	{.a = 0, .b = 1, .c = 2},	//0
	{.a = 0, .b = 2, .c = 3},	//1
	//right
	{.a = 3, .b = 2, .c = 4},	//2
	{.a = 3, .b = 4, .c = 5},	//3
	//back
	{.a = 5, .b = 4, .c = 6},	//4
	{.a = 5, .b = 6, .c = 7},	//5
	//left
	{.a = 7, .b = 6, .c = 1},	//6
	{.a = 7, .b = 1, .c = 0},	//7
	//top
	{.a = 1, .b = 6, .c = 4},	//8
	{.a = 1, .b = 4, .c = 2},	//9
	//bottom
	{.a = 5, .b = 7, .c = 0},	//10
	{.a = 5, .b = 0, .c = 3},	//11
};

void load_cube_mesh_data()
{
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		array_push(mesh.vertices, cube_vertices[i]);
	}

	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		array_push(mesh.faces, cube_faces[i]);
	}
}