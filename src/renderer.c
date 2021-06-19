#include "define.h"

void render_grid(t_vars *vars, int cell_size)
{
	t_render *render = vars->render;

	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

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