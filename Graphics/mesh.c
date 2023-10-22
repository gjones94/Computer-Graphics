#include <stdlib.h>
#include "mesh.h"
#include "graphics.h"
#include "array.h"


vec3d_t* init_mesh_vertices(int num_vertices)
{
	vec3d_t *mesh_vertices = (vec3d_t*) malloc(sizeof(vec3d_t) * num_vertices);

	return mesh_vertices;
}

face_t* init_mesh_faces(int num_faces)
{
	face_t* mesh_faces = (face_t*) malloc(sizeof(face_t) * num_faces);

	return mesh_faces;
}

// Constant vertices for a cube
vec3d_t cube_vertices[N_CUBE_VERTICES] = {
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
face_t cube_faces[N_CUBE_MESH_FACES] = {
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