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

	printf("%f\n", fmod(-5.0, 1.0));

	mlx_loop_hook(vars->render->mlx, update, vars);
	mlx_loop(vars->render->mlx);
	return 0;
}

void init_mlx(t_vars *vars)
{
	vars->render = malloc(sizeof(t_render));
	
	vars->render->mlx = mlx_init();
	vars->render->window = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "Raycast-C");
	vars->render->window_3D = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "3D Render");
}

int update(t_vars *vars)
{
	vars->frame +=1;
	if (vars->frame % 10 != 0)
	{
		return 1;
	}
	render_grid(vars, WIN_HEIGHT / 8);
	//render_raycast(vars);
	render_3D(vars);
	fflush(stdout);
	return 1;
}


char get_map_value(unsigned int *map, int x, int y)
{
	int row = map[y];
	char value = (row >> ((7 - x) * 4)) & 0xf;
	return value;
}