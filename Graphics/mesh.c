#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "graphics.h"
#include "array.h"
#include "colors.h"


//========================================================
//	PROTOTYPES
//========================================================
	static mesh_t* init_mesh();
//========================================================

mesh_t mesh =
{
	.vertices = NULL,
	.num_faces = 0,
	.faces = NULL,
	.num_vertices = 0,
	.rotation = {.x = 0, .y = 0, .z = 0 },
	.scale = {.x = 1, .y = 1, .z = 1 },
	.translation = {.x = 0, .y = 0, .z = 0 }
};

mesh_t* meshes[MAX_NUM_MESHES];
int num_meshes;

bool load_mesh_from_file(const char* filename)
{
	FILE* file;
	char line[200];
	fopen_s(&file, filename, "r");

	mesh_t* mesh = init_mesh();

	if (mesh == NULL)
	{
		printf("Unable to allocate space for mesh");
		return false;
	}

	if (file)
	{
		// Read and print all lines from the file
		while (fgets(line, sizeof(line), file)) {

			char* context = NULL;
			char* token = NULL;
			token = strtok_s(line, " ", &context);

			//If line is vertex
			if (strcmp(token, "v") == 0)
			{
				float vertices[3] = { 0.0, 0.0, 0.0 };
				int vertex_count = 0;

				while (token = strtok_s(NULL, " ", &context))
				{
					vertices[vertex_count++] = strtof(token, NULL);
				}

				vec3_t vertex = { vertices[0], vertices[1], vertices[2] };
				array_push(mesh->vertices, vertex);
				mesh->num_vertices++;
			}
			//else if line is face
			else if(strcmp(token, "f") == 0)
			{
				int base_10 = 10; //used for strtol function Radix to determine base
				int faces[3] = {0,0,0};
				int face_count = 0;

				int index = 0;
				uint32_t face_color = RED;
				while (token = strtok_s(NULL, " ", &context))
				{
					if(index != 3) //add face vertex reference
					{
						faces[face_count++] = strtol(token, NULL, base_10);
						index++;
					}
					else //add color of face
					{
						switch (*token)
						{
							case 'r':
								face_color = RED;
								break;
							case 'g':
								face_color = GREEN;
								break;
							case 'b':
								face_color = BLUE;
								break;
						}
					}
				}

				face_t face = { faces[0], faces[1], faces[2], .color = face_color };
				array_push(mesh->faces, face);
				mesh->num_faces++;
			}
		}
	}
	else 
	{
		printf("Unable to read file %s\n", filename);
	}

	if (num_meshes < MAX_NUM_MESHES)
	{
		meshes[num_meshes++] = mesh;
	}
	else
	{
		printf("Unable to add mesh. Number of maximum meshes has been exceeded.\n");
		return false;
	}

	return true;
}

void rotate_mesh(mesh_t* mesh, float angle, Axis axis)
{
	int num_vertices = mesh->num_vertices;
	switch (axis)
	{
		case X_AXIS:
			mesh->rotation.x += angle;
			break;
		case Y_AXIS:
			mesh->rotation.y += angle;
			break;
		case Z_AXIS:
			mesh->rotation.z += angle;
			break;
	}
}

void free_meshes()
{
	for (int i = 0; i < num_meshes; i++)
	{
		mesh_t* mesh = meshes[i];
		array_free(mesh->faces);
		array_free(mesh->vertices);
		free(mesh);
	}
}

static mesh_t* init_mesh()
{
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));

	if (mesh == NULL)
	{
		return NULL;
	}

	mesh->vertices = NULL;
	mesh->num_faces = 0;
	mesh->faces = NULL;
	mesh->num_faces = 0;
	mesh->rotation.x = 0;
	mesh->rotation.y = 0;
	mesh->rotation.z = 0;
	mesh->scale.x = 1;
	mesh->scale.y = 1;
	mesh->scale.z = 1;

	return mesh;
}