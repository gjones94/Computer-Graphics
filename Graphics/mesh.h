#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"
#include "vector.h"
#include "texture.h"

typedef struct
{
	int a, b, c;
	text2_t a_uv, b_uv, c_uv; // UV coordinate mapping to a texture
	uint32_t color;

} face_t;

typedef struct
{
	vec3_t* vertices;
	int num_vertices;
	face_t* faces;
	int num_faces;
	vec3_t scale;
	vec3_t  rotation; // current rotation of object (x, y, z)
	vec3_t  translation; // current rotation of object (x, y, z)

} mesh_t;

#define MAX_NUM_MESHES 100
extern mesh_t* meshes[MAX_NUM_MESHES];
extern int num_meshes;

bool load_mesh_from_file(const char* filename);
void rotate_mesh(mesh_t* mesh, float angle, Axis axis);
void free_meshes();
