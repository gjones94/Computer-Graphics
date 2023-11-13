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
				int vertices[3] = {0,0,0};
				int uv_coordinates[3] = { 0,0,0 };
				int vertex_count = 0;

				int index = 0;
				uint32_t face_color = RED;
				while (token = strtok_s(NULL, " ", &context))
				{
					if(index != 3) //add face vertex reference
					{
						char* vertex = token;
						char* uv = token + 2;
						vertices[vertex_count] = strtol(vertex, NULL, base_10);
						uv_coordinates[vertex_count++] = strtol(uv, NULL, base_10);
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

				face_t face = { 
					.a = vertices[0], 
					.b = vertices[1], 
					.c = vertices[2], 
					
					// Get the UV mappings of this face from the parsed texture coordinates ("vt" lines in file)
					.a_uv = mesh->texture_uvs[uv_coordinates[0] - 1], 
					.b_uv = mesh->texture_uvs[uv_coordinates[1] - 1], 
					.c_uv = mesh->texture_uvs[uv_coordinates[2] - 1], 
					
					.color = face_color};
				array_push(mesh->faces, face);
				mesh->num_faces++;
			}
			// else if uv coordinates for textures
			else if (strcmp(token, "vt") == 0)
			{
				float uv_coordinates[2] = { 0.0f, 0.0f };
				int uv_count = 0;

				while (token = strtok_s(NULL, " ", &context))
				{
					uv_coordinates[uv_count++] = strtof(token, NULL);
				}

				text2_t uv = { .u = uv_coordinates[0], .v = uv_coordinates[1] };
				array_push(mesh->texture_uvs, uv);
				mesh->num_texture_uvs++;
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
		array_free(mesh->texture_uvs);
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

	mesh->texture_uvs = NULL;
	mesh->num_texture_uvs = 0;

	mesh->rotation.x = 0;
	mesh->rotation.y = 0;
	mesh->rotation.z = 0;
	mesh->scale.x = 1;
	mesh->scale.y = 1;
	mesh->scale.z = 1;
	mesh->translation.x = 0;
	mesh->translation.y = 0;
	mesh->translation.z = 0;

	return mesh;
}