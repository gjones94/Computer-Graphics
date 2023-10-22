#pragma once
#include "vector.h"

typedef struct
{
	int a, b, c;

} face_t;

typedef struct
{
	vec2d_t points[3];

} triangle_t;

#define N_CUBE_VERTICES 8
extern vec3d_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES 6
#define N_TRIANGLES_PER_FACE 2
#define N_CUBE_MESH_FACES (N_CUBE_FACES * N_TRIANGLES_PER_FACE)
extern face_t cube_faces[N_CUBE_MESH_FACES];
extern triangle_t triangles[N_CUBE_MESH_FACES];

vec3d_t* init_mesh_vertices(int num_vertices);
face_t* init_mesh_faces(int num_faces);