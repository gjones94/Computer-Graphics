#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "graphics.h"
#include "array.h"
#include "config.h"

mesh_t mesh =
{
	.vertices = NULL,
	.faces = NULL,
	.rotation = { .x = 0, .y = 0, .z = 0 },
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
	{.a = 1, .b = 2, .c = 3},	//0
	{.a = 1, .b = 3, .c = 4},	//1
	//right
	{.a = 4, .b = 3, .c = 5},	//2
	{.a = 4, .b = 5, .c = 6},	//3
	//back
	{.a = 6, .b = 5, .c = 7},	//4
	{.a = 6, .b = 7, .c = 8},	//5
	//left
	{.a = 8, .b = 7, .c = 2},	//6
	{.a = 8, .b = 2, .c = 1},	//7
	//top
	{.a = 2, .b = 7, .c = 5},	//8
	{.a = 2, .b = 5, .c = 3},	//9
	//bottom
	{.a = 6, .b = 8, .c = 1},	//10
	{.a = 6, .b = 1, .c = 4},	//11
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

void load_mesh_from_file(const char* filename)
{
	FILE* file;
	char line[200];
	fopen_s(&file, filename, "r");

	if (file)
	{
		// Read and print all lines from the file
		while (fgets(line, sizeof(line), file)) {

			char* context = NULL;
			char* token = NULL;
			token = strtok_s(line, " ", &context);
			if (strcmp(token, "v") == 0)
			{
				float vertices[3] = { 0.0, 0.0, 0.0 };
				int vertex_count = 0;

				while (token = strtok_s(NULL, " ", &context))
				{
					vertices[vertex_count++] = strtof(token, NULL);
				}

				vec3_t vertex = { vertices[0], vertices[1], vertices[2] };
				array_push(mesh.vertices, vertex);
			}
			else if(strcmp(token, "f") == 0)
			{
				int base_10 = 10;
				int faces[3] = {0,0,0};
				int face_count = 0;

				while (token = strtok_s(NULL, " ", &context))
				{
					faces[face_count++] = strtol(token, NULL, base_10);
				}

				face_t face = { faces[0], faces[1], faces[2] };
				array_push(mesh.faces, face);
			}
		}
	}
	else 
	{
		printf("Unable to read file %s\n", filename);
	}
	
	mesh.rotation.z = 180;
	mesh.color = BLUE_GREEN;
}

void rotate_mesh(float angle, Axis axis)
{
	int num_vertices = array_length(mesh.vertices);
	switch (axis)
	{
		case X_AXIS:
			mesh.rotation.x += angle;
			break;
		case Y_AXIS:
			mesh.rotation.y += angle;
			break;
		case Z_AXIS:
			mesh.rotation.z += angle;
			break;
	}
}
