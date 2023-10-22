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

#define ROTATION_INCREMENT .01
extern float current_rotation;

#define N_VERTICES 8
extern vec3d_t mesh_vertices[N_VERTICES];

#define N_CUBE_FACES 6
#define N_TRIANGLES_PER_FACE 2
#define N_MESH_FACES (N_CUBE_FACES * N_TRIANGLES_PER_FACE)
extern face_t mesh_faces[N_MESH_FACES];
extern triangle_t triangles[N_MESH_FACES];

vec3d_t* init_mesh_vertices(int num_vertices);
face_t* init_mesh_faces(int num_faces);
triangle_t* get_triangle_meshes();
