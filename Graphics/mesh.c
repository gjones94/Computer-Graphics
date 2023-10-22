#include <stdlib.h>
#include "mesh.h"
#include "graphics.h"

float current_rotation = 0.0f;

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

vec3d_t mesh_vertices[N_VERTICES] = {
	{ .x = -1, .y = -1, .z = -1},	//0
	{ .x = -1, .y =  1, .z = -1},	//1
	{ .x =  1, .y =  1, .z = -1},	//2
	{ .x =  1, .y = -1, .z = -1},	//3
	{ .x =  1, .y =  1, .z =  1},	//4
	{ .x =  1, .y = -1, .z =  1},	//5
	{ .x = -1, .y =  1, .z =  1},	//6
	{ .x = -1, .y = -1, .z =  1},	//7
};

face_t mesh_faces[N_MESH_FACES] = {
	{.a = 0, .b = 1, .c = 2},	//0
	{.a = 0, .b = 2, .c = 3},	//1
	{.a = 3, .b = 2, .c = 4},	//2
	{.a = 3, .b = 4, .c = 5},	//3
	{.a = 5, .b = 4, .c = 6},	//4
	{.a = 5, .b = 6, .c = 7},	//5
	{.a = 7, .b = 6, .c = 1},	//6
	{.a = 7, .b = 1, .c = 0},	//7
	{.a = 1, .b = 6, .c = 4},	//8
	{.a = 1, .b = 4, .c = 2},	//9
	{.a = 5, .b = 7, .c = 0},	//10
	{.a = 5, .b = 0, .c = 3},	//11
};

static void update_movement()
{
	current_rotation += ROTATION_INCREMENT;
	if (current_rotation >= 360)
	{
		current_rotation = 0.0f;
	}
}

triangle_t* get_triangle_meshes()
{
	update_movement();

	triangle_t triangles_to_render[N_MESH_FACES];

	for (int i = 0; i < N_MESH_FACES; i++)
	{
		vec3d_t face_vertices[3];

		// Get each vertex for face
		face_vertices[0] = mesh_vertices[mesh_faces[i].a];
		face_vertices[1] = mesh_vertices[mesh_faces[i].b];
		face_vertices[2] = mesh_vertices[mesh_faces[i].c];

		triangle_t projected_triangle;
		vec3d_t transformed_vertex;
		for (int j = 0; j < 3; j++)
		{
			// Rotate each vertex in face
			transformed_vertex = rotate(face_vertices[j], current_rotation, X_AXIS);
			transformed_vertex = rotate(transformed_vertex, current_rotation, Y_AXIS);
			transformed_vertex = rotate(transformed_vertex, current_rotation, Z_AXIS);

			// Move point away from camera
			transformed_vertex.z -= camera_position.z;

			// Project point into 2d space
			vec2d_t projected_vertex = project_2d(transformed_vertex);

			// Save triangle mesh with projected points
			projected_triangle.points[j] = projected_vertex;
		}

		// Add triangle for rendering
		triangles_to_render[i] = projected_triangle;
	}

	return triangles_to_render;
}
