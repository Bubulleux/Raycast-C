#include <mlx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//----------Const----------
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define FOV 2

#define DEGRE 57.2958

//----------Struct----------

typedef struct s_vector
{
	double x;
	double y;
} t_vector;

typedef struct s_render
{
	void *mlx;
	void *window;
} t_render;

typedef struct s_vars
{
	t_render *render;
	int frame;
	unsigned int *map;
} t_vars;

typedef struct s_img
{
	void *img;
	void *addr;
	int bit_per_pixel;
	int size_line;
	int endian;
}t_img;

//----------Function----------

//main.c
void init_mlx(t_vars *vars);
int update(t_vars *vars);
char get_map_value(unsigned int *map, int x, int y);

//renderer.c
void render_grid(t_vars *vars, int cell_size);
void set_pixel_img(t_img *img, int x, int y, int color);
void render_dot(t_vars *vars, int x, int y, int color);

//raycast.c
t_vector make_raycast(t_vars *vars, t_vector origin, t_vector dir);
void render_raycast(t_vars *vars);
bool collide_face(t_vars *vars, double x, double y);

//vector.c
t_vector vector_get_normal(t_vector vector);
double vector_get_magnetude(t_vector vector);
t_vector new_vector(double x, double y);
t_vector get_intersection(double a, double b, double a_prime, double b_prime);