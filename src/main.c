#include "define.h"

unsigned int map[8] = {
	0x0F000F00,
	0x00FFF000,
	0x00000000,
	0xF0000000,
	0xF0000000,
	0xF00000F0,
	0x00FF00F0,
	0x00FFF000,
};

int main()
{  
	t_vars *vars = malloc(sizeof(t_vars));
	vars->map = malloc(sizeof(unsigned int) * 8);
	for (int i; i < 8; i++)
	{
		vars->map[i] = map[i];
	}
	init_mlx(vars);
	init_input(vars);
	init_player(vars);

	printf("%f\n", fmod(-5.0, 1.0));

	mlx_loop_hook(vars->render->mlx, update, vars);
	mlx_loop(vars->render->mlx);
	return 0;
}

void init_mlx(t_vars *vars)
{
	vars->render = malloc(sizeof(t_render));
	
	vars->render->mlx = mlx_init();
	vars->render->window_3D = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "3D Render");
	vars->render->window = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "Raycast-C");
}

int update(t_vars *vars)
{
	vars->frame +=1;

	if (vars->last_time_update == 0)
		vars->last_time_update = current_timestamp();
	
	vars->delta_time = (current_timestamp() - vars->last_time_update) / 1000.0;
	
	// printf("\r");
	// for (int i = 0; i < 4; i++)
	// {
	// 	printf("%d ", vars->input_vars->inputs_pressed[i][1]);
	// }
	if (vars->delta_time < (1.0 / FPS)) return 0;
	vars->last_time_update = current_timestamp();
	update_player(vars->input, vars->player, vars->delta_time);
	//render_raycast(vars);
	render_3D(vars);
	//printf("\r %d %d %d %d", vars->input->up_pressed, vars->input->down_pressed, vars->input->left_pressed, vars->input->right_pressed);
	fflush(stdout);
	return 1;
}


char get_map_value(unsigned int *map, int x, int y)
{
	int row = map[y];
	char value = (row >> ((7 - x) * 4)) & 0xf;
	return value;
}

long long current_timestamp()
{
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}
