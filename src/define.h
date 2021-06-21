#include <mlx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//----------Const----------
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define FOV 0

#define DEGRE 57.2958

#define SPEED 0.5
#define FPS 60

//Color
#define SKY_COLOR 0x00ffff
#define WALL_COLOR_UP 0xff0000
#define WALL_COLOR_DOWN 0xaa0000
#define WALL_COLOR_LEFT 0x7700000
#define WALL_COLOR_RIGHT 0x330000
#define GROUND_COLOR 0x333333

//Input
#define UP_ARROW 65362
#define DOWN_ARROW 65364
#define LEFT_ARROW 65361
#define RIGHT_ARROW 65363

//----------Struct----------

typedef struct s_img
{
	void *img;
	void *addr;
	int bit_per_pixel;
	int size_line;
	int endian;
}t_img;

typedef struct s_vector
{
	double x;
	double y;
} t_vector;

typedef struct s_input
{
	int up_pressed;
	int down_pressed;
	int left_pressed;
	int right_pressed;
} t_input;

typedef struct s_player
{
	t_vector position;
	double angle;
} t_player;


typedef struct s_render
{
	void *mlx;
	void *window;
	void *window_3D;
} t_render;

typedef struct s_vars
{
	int frame;
	unsigned int *map;
	double last_time_update;
	double delta_time;

	t_input *input;
	t_render *render;
	t_player *player;
} t_vars;


typedef struct s_raycast
{
	t_vector hit_pos;
	double hit_dist;
	int hit_color;
} t_raycast;


//----------Function----------

//main.c
void init_mlx(t_vars *vars);
int update(t_vars *vars);
char get_map_value(unsigned int *map, int x, int y);
long long current_timestamp();

//renderer.c
void render_grid(t_vars *vars, int cell_size);
void set_pixel_img(t_img *img, int x, int y, int color);
void render_dot(t_vars *vars, int x, int y, int color);
void render_3D(t_vars *vars);

//raycast.c
t_raycast calc_raycast(t_vars *vars, t_vector origin, t_vector dir);
void render_raycast(t_vars *vars);
int collide_face(t_vars *vars, double x, double y);

//vector.c
t_vector vector_get_normal(t_vector vector);
double vector_get_magnetude(t_vector vector);
t_vector new_vector(double x, double y);
t_vector get_intersection(double a, double b, double a_prime, double b_prime);

//input.c
int key_pressed(int key, t_input *input);
int key_relased(int key, t_input *input);
void init_input(t_vars *vars);

//player.c
void update_player(t_input *input, t_player *player, double deltat_ime);
void init_player(t_vars *vars);