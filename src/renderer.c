#include "define.h"

void init_mlx(t_vars *vars)
{
	vars->render = malloc(sizeof(t_render));
	
	vars->render->mlx = mlx_init();
	vars->render->window_3D = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "3D Render");
	//vars->render->window = mlx_new_window(vars->render->mlx, WIN_WIDTH, WIN_HEIGHT, "Raycast-C");

	t_render *render = vars->render;
	int size;

	render->wall_img = load_img(render->mlx, "texture/wall.xpm", &size, &size);
	render->box_img = load_img(render->mlx, "texture/box.xpm", &size, &size);;
    render->ground_img = load_img(render->mlx, "texture/ground.xpm", &size, &size);
    render->player_map_img = load_img(render->mlx, "texture/player_map.xpm", &size, &size);
}

t_img* load_img(void* mlx, char* file_name, int *size_x, int *size_y)
{
    t_img *img = malloc(sizeof(t_img));
    img->img = mlx_xpm_file_to_image(mlx, file_name, size_x, size_y);
    img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);
    return img;
}

void render_grid(t_vars *vars, int cell_size)
{
	t_render *render = vars->render;

	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

	
	for (int y = 0; y < vars->height_m; y++)
	{
		for (int x = 0; x < vars->width_m; x++)
		{
			if (get_map_value(vars, x, y, 0xF) == 0xF)
			{
				for (int _y = 0; _y < WIN_HEIGHT / vars->height_m; _y++)
				{
					for (int _x = 0; _x < WIN_WIDTH / vars->width_m; _x++)
					{
						set_pixel_img(img, x * WIN_WIDTH / vars->width_m + _x, y * WIN_HEIGHT / vars->height_m + _y, 0x00333333);
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
    //return;
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
//	t_vector mouse_pos;
//	int x;
//	int y;
//	mlx_mouse_get_pos(vars->render->mlx, vars->render->window, &x, &y);
//	mouse_pos.x = ((double)x / (double)WIN_WIDTH * vars->width_m) - vars->player->position.x;
//	mouse_pos.y = ((double)y / (double)WIN_HEIGHT * vars->height_m) - vars->player->position.y;
//	//printf("\r%f %f    %f %f             ", mouse_pos.x, mouse_pos.y, x / (double)WIN_WIDTH, y / (double)WIN_HEIGHT);
//	t_vector dir = vector_get_normal(mouse_pos);
//	double angle_dir = acos(dir.y) * DEGRE * (dir.x < 0 ? -1 : 1);
	double angle_dir = vars->player->angle_x;
	
	//if (vars->player->angle_x == angle_dir) return;

	//render_grid(vars, WIN_HEIGHT / vars->height_m);

	t_render *render = vars->render;
	t_img *img = malloc(sizeof(t_img));
	img->img = mlx_new_image(render->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		double add_angle = ((double)x * (double)FOV / (double)WIN_WIDTH) - FOV / 2;
		double cur_angle = angle_dir - add_angle;

		t_raycast raycast = calc_raycast(vars, vars->player->position, new_vector(sin((cur_angle) / DEGRE), cos((cur_angle) / DEGRE)));

		//double view_hit_ground_dist = tan((90 - (FOV_VERTICAL / 2)) / DEGRE) * PLAYER_HEIGHT;

		//int wall_height = (view_hit_ground_dist / raycast.hit_dist) * 800;

		double wall_angle_see = 90 - (atan(raycast.hit_dist / PLAYER_HEIGHT) * DEGRE);
		//wall_angle_see /= cos(add_angle / DEGRE);
		//double ground_angle_see = (FOV_VERTICAL - wall_angle_see) / 2;

		//int ground_height = (WIN_WIDTH - wall_height) / 2;

		for (int y = 0; y < WIN_HEIGHT; y++)
		{
		    double cur_angle_vertical = y * (FOV_VERTICAL * cos(add_angle / DEGRE)) / (double)WIN_HEIGHT - (FOV_VERTICAL * cos(add_angle / DEGRE)) / 2  + vars->player->angle_y;
		    //cur_angle_vertical /= cos(add_angle / DEGRE);
		    //printf("%f\n", cur_angle_vertical);
			int color;
			if (cur_angle_vertical < -wall_angle_see) color = SKY_COLOR;
			else if (cur_angle_vertical < wall_angle_see)
			{
			    if (raycast.hit_dist >= FOG_FULL)
                {
                    color = SKY_COLOR;
                }
			    else
                {
                    char wall_type = get_map_value(vars, raycast.map_box_hit.x, raycast.map_box_hit.y, 0xF0);
                    t_img *wall_img = NULL;
                    switch (wall_type) {
                        case 0x00:
                            wall_img = render->wall_img;
                            break;
                        case 0x10:
                            wall_img = render->box_img;
                            break;
                        default:
                            wall_img = render->wall_img;
                            break;

                    }
                    color = get_pixel_img(wall_img, (int)(raycast.x_hit * TEXTURE_SIZE), (int)((cur_angle_vertical + wall_angle_see) / (wall_angle_see * 2) * TEXTURE_SIZE));
                    color = multiplie_color(color, raycast.hit_color);
                    if (raycast.hit_dist >= FOG_START)
                    {
                        color = color_mean(SKY_COLOR, color, (raycast.hit_dist - FOG_START) / (FOG_FULL - FOG_START));
                    }

                    //color = 0xff0000;
                    //printf(" y: %d %d %f,", y, color, raycast.hit_color);
                }
			}
			else
			{
			    double angle = 90 - (cur_angle_vertical);
			    if (angle <= 0)
                {
                    continue;
                }
			    //angle = atan(raycast.hit_dist / PLAYER_HEIGHT) * DEGRE;
			    //angle /= cos(add_angle / DEGRE);
			    double dist_groud = tan(angle / DEGRE) * PLAYER_HEIGHT/* / cos(add_angle / DEGRE)*/;
                if (dist_groud >= FOG_FULL)
                {
                    color = SKY_COLOR;
                }
                else
                {
                    //int groud_pixel_index = y - (ground_height + wall_height);
                    //dist_groud = raycast.hit_dist - (groud_pixel_index * (raycast.hit_dist - view_hit_ground_dist) / (double)ground_height);
                    t_vector pixel_pos = new_vector(vars->player->position.x + sin(cur_angle / DEGRE) * dist_groud,
                                                    vars->player->position.y + cos(cur_angle / DEGRE) * dist_groud);

                    color = get_pixel_img(render->ground_img,   abs((int)(fmod(pixel_pos.x, 1.0) * TEXTURE_SIZE)),
                                                                abs((int)(fmod(pixel_pos.y, 1.0) * TEXTURE_SIZE)));
                    if (dist_groud >= FOG_START)
                    {
                        color = color_mean(SKY_COLOR, color, (dist_groud - FOG_START) / (FOG_FULL - FOG_START));
                    }
                    //color = GROUND_COLOR;
                }

			}
			set_pixel_img(img, x, y, color);
			//printf("set pixel color %d\n", color);
		}
		//printf("\n");
		//printf("\rx_hit %f", raycast.x_hit);
	}

	if (vars->input->enter_pressed == 1)
    {
        render_mini_map(vars, img);
    }
	mlx_put_image_to_window(render->mlx, render->window_3D, img->img, 0, 0);
	mlx_destroy_image(render->mlx, img->img);
	free(img);

}

int color_mean(int color_a, int color_b, double coef_a)
{
//    printf("\n");
//    printf("%06x %06x %f\n", color_a, color_b, coef_a);
    char R = (char)(((color_a >> 16) & 0xFF) * coef_a + ((color_b >> 16) & 0xFF) * (1 - coef_a));
    char G = (char)(((color_a >> 8) & 0xFF) * coef_a + ((color_b >> 8) & 0xFF) * (1 - coef_a));
    char B = (char)(((color_a >> 0) & 0xFF) * coef_a + ((color_b >> 0) & 0xFF) * (1 - coef_a));
//    printf("%02x %02x %02x\n", (R & 0xFF), (G & 0xFF), (B & 0xFF));
    int out = (R & 0xFF);
    out = (out << 8) | (G & 0xFF);
    out = (out << 8) | (B & 0xFF);
//    printf("%02x%02x%02x %06x\n", (R & 0xFF), (G & 0xFF), (B & 0xFF), out);
    return out;
}

void render_mini_map(t_vars *vars, t_img *img)
{
    t_render *render = vars->render;
    int cell_size = (int)(WIN_WIDTH / vars->width_m * 0.8);
    int img_width = cell_size * vars->width_m;
    int img_height = cell_size * vars->height_m;
    int offset_x = (WIN_WIDTH - img_width) / 2;
    int offset_y = (WIN_HEIGHT - img_height) / 2;


    for (int y = 0; y < img_height; y++)
    {
        for (int x = 0; x < img_width; x++)
        {
            int color = 0xff00ff;
            if ((x % cell_size) < 2 || (x % cell_size) >= (cell_size - 2) || (y % cell_size) < 2 || (y % cell_size) >= (cell_size - 2))
            {
                color = 0x0;
            }
            else
            {
                t_img *img = NULL;
                switch (get_map_value(vars, x * vars->width_m / img_width, y * vars->height_m / img_height, 0xFF)) {
                    case 0x0F:
                        img = render->wall_img;
                        break;
                    case 0x1F:
                        img = render->box_img;
                        break;
                    case 0x00:
                        img = render->ground_img;
                        break;
                }
                color = get_pixel_img(img, (x % cell_size) * TEXTURE_SIZE / cell_size, (y % cell_size) * TEXTURE_SIZE / cell_size);
            }
            set_pixel_img(img, offset_x + x, offset_y + y, color);
        }
    }

    for (int _y = 0; _y < cell_size; _y++)
    {
        for (int _x = 0; _x < cell_size; _x++)
        {
            int color;
            int x = (vars->player->position.x * img_width / vars->width_m) - cell_size / 2 + offset_x + _x;
            int y = (vars->player->position.y * img_height / vars->height_m) - cell_size / 2 + offset_y + _y;
            double magnetude = vector_get_magnetude(new_vector(_x - cell_size / 2, _y - cell_size / 2));

            if (magnetude > cell_size * 0.35 || x < offset_x || x >= offset_x + img_width || y < offset_y || y >= offset_y + img_height)
            {
                //printf("%d %d          %d %d ", x, y, _x, _y);
                //printf("continue\n");
                continue;
            }
            else if (magnetude > cell_size * 0.3)
            {
                color = 0x999999;
            }
            else
            {
                color = 0xeeeeee;
            }
            //printf("Pixel Render \n");
            set_pixel_img(img, x, y, color);
        }
    }
    //printf("MiniMap has been rendered\n");

}

t_img* new_img(void* mlx, int width, int height)
{
    t_img *img = malloc(sizeof(t_img));
    img->img = mlx_new_image(mlx, width, height);
    img->addr = mlx_get_data_addr(img->img, &img->bit_per_pixel, &img->size_line, &img->endian);
    return img;
}
void free_img(void *mlx, t_img *img)
{
    mlx_destroy_image(mlx, img->img);
    free(img);
}

void free_render(t_render *render)
{
    mlx_destroy_window(render->mlx, render->window_3D);
    free_img(render->mlx, render->ground_img);
    free_img(render->mlx, render->box_img);
    free_img(render->mlx, render->wall_img);
    free_img(render->mlx, render->player_map_img);
    mlx_destroy_display(render->mlx);
    free(render);
}
