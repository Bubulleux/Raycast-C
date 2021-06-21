#include "define.h"

void render_grid(t_vars *vars, int cell_size)
{
	t_render *render = vars->render;

	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

	
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (get_map_value(vars->map, x, y))
			{
				for (int _y = 0; _y < WIN_HEIGHT / 8; _y++)
				{
					for (int _x = 0; _x < WIN_WIDTH / 8; _x++)
					{
						set_pixel_img(img, x * WIN_WIDTH / 8 + _x, y * WIN_HEIGHT / 8 + _y, 0x00333333);
					}
				}
			}
		}
	}

	for (int y = 0; y < WIN_HEIGHT; y++)
	{
		for (int x = 0; x < WIN_WIDTH; x++)
		{
			if (x % cell_size <= 2 || y % cell_size <= 2 || y == 0 || x == 0)
			{
				set_pixel_img(img, x, y, 0x00aaaaaa);
			}
		}
	}

	mlx_put_image_to_window(render->mlx, render->window, img->img, 0, 0);
	mlx_destroy_image(render->mlx, img->img);
	free(img);
}

void set_pixel_img(t_img *img, int x, int y, int color)
{
	char **dst;
	dst = malloc(sizeof(char*));
	*dst = img->addr + (y * img->size_line + x * (img->bit_per_pixel / 8));
	*(unsigned int*)*dst = color;
	free(dst);
}

void render_dot(t_vars *vars, int x, int y, int color)
{
	for (int _y = -2; _y <= 2; _y++)
	{
		for (int _x = -2; _x <= 2; _x++)
		{
			if (x + _x < 0 || y + _y < 0 || x + _x >= WIN_WIDTH || y + _y >= WIN_HEIGHT)
			{
				continue;
			}
			mlx_pixel_put(vars->render->mlx, vars->render->window, x + _x, y + _y, color);
		}
	}
}

void render_3D(t_vars *vars)
{

	t_vector mouse_pos;
	int x;
	int y;
	mlx_mouse_get_pos(vars->render->mlx, vars->render->window, &x, &y);
	mouse_pos.x = ((double)x / (double)WIN_WIDTH * 8) - vars->player->position.x;
	mouse_pos.y = ((double)y / (double)WIN_HEIGHT * 8) - vars->player->position.y;
	//printf("\r%f %f    %f %f             ", mouse_pos.x, mouse_pos.y, x / (double)WIN_WIDTH, y / (double)WIN_HEIGHT);
	t_vector dir = vector_get_normal(mouse_pos);
	double angle_dir = acos(dir.y) * DEGRE * (dir.x < 0 ? -1 : 1);
	
	if (vars->player->angle == angle_dir) return;
	vars->player->angle = angle_dir;
	render_grid(vars, WIN_HEIGHT / 8);

	t_render *render = vars->render;
	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		double add_angle = ((double)x * (double)FOV / (double)WIN_HEIGHT) - FOV / 2;
		double cur_angle = angle_dir + add_angle;
		t_raycast raycast = calc_raycast(vars, vars->player->position, new_vector(sin((cur_angle) / DEGRE), cos((cur_angle) / DEGRE)));
		int wall_height = (int)((double)WIN_HEIGHT / raycast.hit_dist / cos(add_angle / DEGRE));
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			int color;
			if (y < (WIN_WIDTH - wall_height) / 2) color = SKY_COLOR;
			else if (y < ((WIN_WIDTH - wall_height) / 2) + wall_height) color = raycast.hit_color;
			else color = GROUND_COLOR;
			set_pixel_img(img, x, y, color);
			//printf("set pixel color %d\n", color);
		}
	}
	mlx_put_image_to_window(render->mlx, render->window_3D, img->img, 0, 0);
	mlx_destroy_image(render->mlx, img->img);
	free(img);

}

