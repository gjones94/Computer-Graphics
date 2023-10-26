#pragma once
#include <stdint.h>
#include "vector.h"

typedef struct
{
	int a, b, c;

} face_t;

typedef struct
{
	vec2_t points[3];

} triangle_t;

typedef struct
{
	vec3_t* vertices;
	face_t* faces;
	vec3_t  rotation; // current rotation of object (x, y, z)
	uint32_t color;
} mesh_t;

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)
extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];
extern triangle_t triangles[N_CUBE_FACES];

extern mesh_t mesh; //global mesh instance

void load_cube_mesh_data();
void load_mesh_from_file(const char* filename);
void rotate_mesh(float angle, Axis axis);
