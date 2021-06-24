#include "define.h"

void init_mlx(t_vars *vars)
{
	vars->render = malloc(sizeof(t_render));
	
	vars->render->mlx = mlx_init();
	vars->render->window_3D = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "3D Render");
	vars->render->window = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "Raycast-C");

	t_render *render = vars->render;
	render->wall_img = malloc(sizeof(t_img));
	int size;
	render->wall_img->img = mlx_xpm_file_to_image(render->mlx, "texture/wall.xpm", &size, &size);
	render->wall_img->addr = mlx_get_data_addr(render->wall_img->img, &render->wall_img->bit_per_pixel, &render->wall_img->size_line, &render->wall_img->endian);
}

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
	mlx_put_image_to_window(render->mlx, render->window, render->wall_img->img, 0, 0);
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

int get_pixel_img(t_img *img, int x, int y)
{
	char **dst;
	dst = malloc(sizeof(char*));
	*dst = img->addr + (y * img->size_line + x * (img->bit_per_pixel / 8));
	int color = *(unsigned int*)*dst;
	free(dst);
	return color;
}

int multiplie_color(int color, double multiple)
{
	unsigned int color_out = 0x0;
	for(int i = 0; i < 3; i++)
	{
		char value = (char)((double)((color >> i * 8) & 0xFF) * multiple);
		color_out >>= 1 * 8;
		color_out = color_out | (value << 2 * 8);
		//printf("%x  %x %x\n", color_out, value, color >> i * 8);
	}
	return color_out;
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
	angle_dir = vars->player->angle;
	
	//if (vars->player->angle == angle_dir) return;

	render_grid(vars, WIN_HEIGHT / 8);

	t_render *render = vars->render;
	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		double add_angle = ((double)x * (double)FOV / (double)WIN_HEIGHT) - FOV / 2;
		double cur_angle = angle_dir - add_angle;
		t_raycast raycast = calc_raycast(vars, vars->player->position, new_vector(sin((cur_angle) / DEGRE), cos((cur_angle) / DEGRE)));
		int wall_height = (int)((double)WIN_HEIGHT / raycast.hit_dist / cos(add_angle / DEGRE));
		int ground_height = (WIN_WIDTH - wall_height) / 2;
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			int color;
			if (y < ground_height) color = SKY_COLOR;
			else if (y < ground_height + wall_height) 
			{
				color = get_pixel_img(render->wall_img,  (int)(raycast.x_hit * TEXTURE_SIZE), (int)((y - ground_height) / (double)wall_height * TEXTURE_SIZE));
				color = multiplie_color(color, raycast.hit_color);
				//printf("%d", (int)((y - ground_height) / (double)wall_height * TEXTURE_SIZE));
			}
			else color = GROUND_COLOR;
			set_pixel_img(img, x, y, color);
			//printf("set pixel color %d\n", color);
		}
		//printf("\n");
		//printf("\rx_hit %f", raycast.x_hit);
	}
	mlx_put_image_to_window(render->mlx, render->window_3D, img->img, 0, 0);
	mlx_destroy_image(render->mlx, img->img);
	free(img);

}

