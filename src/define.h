#include <mlx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

//----------Const----------
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define FOV 50
#define FOV_VERTICAL 50
#define PLAYER_HEIGHT 0.5

#define DEGRE 57.2958

#define SPEED 1
#define COLIDE_BOX_SIZE 0.1

#define FPS 60

#define MAP "map.txt"

//Color
#define SKY_COLOR 0x83b9de
#define WALL_COLOR_UP 1
#define WALL_COLOR_DOWN 0.3
#define WALL_COLOR_LEFT 0.6
#define WALL_COLOR_RIGHT 0.9
#define GROUND_COLOR 0x333333
#define TEXTURE_SIZE 16

#define FOG_START 7
#define FOG_FULL 12

//Input
#define UP_ARROW 65362
#define DOWN_ARROW 65364
#define LEFT_ARROW 65361
#define RIGHT_ARROW 65363
#define MINI_MAP_INPUT 65293
#define ESCAPE_BTN 65307
#define SENSITIVITY 3.0


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
    int enter_pressed;
    char lock_mouse;
	t_vector mouse_pos;
	t_vector mouse_delta;
} t_input;

typedef struct s_player
{
	t_vector position;
	double angle_x;
    double angle_y;
} t_player;


typedef struct s_render
{
	void *mlx;
	void *window;
	void *window_3D;
	t_img *wall_img;
    t_img *box_img;
    t_img *ground_img;
    t_img *player_map_img;
} t_render;

typedef struct s_vars
{
	int frame;
	char **map;
	int height_m;
	int width_m;
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
	double x_hit;
	double hit_color;
	t_vector map_box_hit;
} t_raycast;


//----------Function----------

//main.c
int update(t_vars *vars);
char get_map_value(t_vars *vars, double x, double y, char layer);
long long current_timestamp();
void free_all(t_vars *vars);

//renderer.c
void init_mlx(t_vars *vars);
void render_grid(t_vars *vars, int cell_size);
void set_pixel_img(t_img *img, int x, int y, int color);
void render_dot(t_vars *vars, int x, int y, int color);
void render_3D(t_vars *vars);
int get_pixel_img(t_img *img, int x, int y);
int multiplie_color(int color, double multiple);
t_img* load_img(void* mlx, char* file_name, int *size_x, int *size_y);
void render_mini_map(t_vars *vars, t_img *img);
t_img* new_img(void* mlx, int width, int height);
int color_mean(int color_a, int color_b, double coef_a);
void free_img(void *mlx, t_img *img);
void free_render(t_render *render);

//raycast.c
t_raycast calc_raycast(t_vars *vars, t_vector origin, t_vector dir);
void render_raycast(t_vars *vars);
double collide_face(t_vars *vars, double x, double y);
t_vector get_map_cell_colide(t_vars *vars, double _x, double _y, double color);

//vector.c
t_vector vector_get_normal(t_vector vector);
double vector_get_magnetude(t_vector vector);
t_vector new_vector(double x, double y);
t_vector get_intersection(double a, double b, double a_prime, double b_prime);
void set_vector(t_vector *vector, double x, double y);

//input.c
int key_pressed(int key, t_vars *vars);
int key_relased(int key, t_input *input);
void init_input(t_vars *vars);
void update_mouse(t_input *input, t_vars *vars);

//player.c
void update_player(t_vars *vars, t_input *input, t_player *player, double deltat_ime);
void init_player(t_vars *vars);
bool check_collide(t_vars *vars, t_vector pos, double box_size);

//map_parser.c
char* parse_file(char *file_name);
char*** parse_line(char *txt, int size);
char* clean_file(char* txt, int size, int *end_size_ptr);
char* resize_char(char *txt, int size);
void resize_char_ptr(char *src, char *ptr, int size);
void count_txt_elemente(char *txt, int txt_size, int *line_count_ptr, int **elemente_count_ptr);
char** parse_map_file(char *file_name, t_vars *vars);