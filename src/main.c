#include "define.h"

int main()
{  
	t_vars *vars = malloc(sizeof(t_vars));

	init_mlx(vars);

	mlx_loop_hook(vars->render->mlx, update, vars);
	mlx_loop(vars->render->mlx);
	return 0;
}

void init_mlx(t_vars *vars)
{
	vars->render = malloc(sizeof(t_render));
	
	vars->render->mlx = mlx_init();
	vars->render->window = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "Raycast-C");
}

int update(t_vars *vars)
{
	vars->frame +=1;
	if (vars->frame % 500 != 0)
	{
		return 1;
	}
	render_grid(vars, 40);
	make_raycast(vars, 40);
	fflush(stdout);
	return 1;
}

