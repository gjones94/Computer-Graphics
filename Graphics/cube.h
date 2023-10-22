
typedef struct
{
	vec3d_t* vertices;
	int num_vertices;

} cube3d_t;

vec3d_t* vertices;
cube3d_t* create_cube(float origin_x, float origin_y, float origin_z, float size, float point_spacing);
void print_vertices(vec3d_t* points, int num_points);
void print_vertex(vec3d_t point);
