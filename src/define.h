#include <mlx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//----------Const----------
#define WIN_WIDTH 800
#define WIN_HEIGHT 800



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

//renderer.c
void render_grid(t_vars *vars, int cell_size);
void set_pixel_img(t_img *img, int x, int y, int color);
void render_dot(t_vars *vars, int x, int y, int color);

//raycast.c
void make_raycast(t_vars *vars, double grid_size);

//vector.c
t_vector vector_get_normal(t_vector vector);
double vector_get_magnetude(t_vector vector);
t_vector new_vector(double x, double y);
t_vector get_intersection(double a, double b, double a_prime, double b_prime);