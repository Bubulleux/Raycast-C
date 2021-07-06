#include "define.h"

void render_raycast(t_vars *vars)
{
	t_vector mouse_pos;
	int x;
	int y;
	mlx_mouse_get_pos(vars->render->mlx, vars->render->window, &x, &y);
	mouse_pos.x = x - 400;
	mouse_pos.y = y - 400;
	t_vector dir = vector_get_normal(mouse_pos);
	double angle_dir = acos(dir.y) * DEGRE * (dir.x < 0 ? -1 : 1);
	printf("\r%f %f   %f", asin(dir.x) * DEGRE, acos(dir.y) * DEGRE, angle_dir);
	 for (int i = -FOV / 2; i < FOV / 2; i++)
	 {
	 	t_raycast raycast = calc_raycast(vars, new_vector(4, 4), new_vector(sin((angle_dir + i) / DEGRE), cos((angle_dir + i) / DEGRE)));
	 	render_dot(vars, (int)(raycast.hit_pos.x * WIN_WIDTH / vars->width_m), (int)(raycast.hit_pos.y * WIN_HEIGHT / vars->height_m), 0x00ff00);
	 }
	 t_raycast dote = calc_raycast(vars, new_vector(4, 4), new_vector(sin((angle_dir) / DEGRE), cos((angle_dir) / DEGRE)));
	 render_dot(vars, (int)(dote.hit_pos.x * WIN_WIDTH / vars->width_m), (int)(dote.hit_pos.y * WIN_HEIGHT / vars->height_m), 0x00ff00);
}

t_raycast calc_raycast(t_vars *vars, t_vector origin, t_vector dir)
{
	
	t_vector pos = new_vector(0, 0);
	t_raycast raycast;
	//printf("%f %f\n", origin.x, origin.y);
	//render_dot(vars, origin.x / vars->width_m * WIN_WIDTH, origin.y / vars->height_m * WIN_HEIGHT, 0xffff00);

	if (dir.y == 0 || dir.x == 0) return raycast;

	double slop_x = dir.y / dir.x;
	double b_x = origin.y - (origin.x * slop_x);
	b_x = fmod(origin.x, 1);

	double slop_y = dir.x / dir.y;
	double b_y = origin.x - (origin.y * slop_y);
	b_y = fmod(origin.y, 1);

	int i_x = 0;
	int i_y = 0;

    printf("\r");
	for (int i; i < 20; i++)
	{
		//double next_line_y =  floor(pos.y + ((dir.y < 0 && fmod(abs(pos.y + b_y), 1.0) == 0) ? -0.01 : 0) + 0) + (dir.y < 0? 0 : 1) - b_y;
		double next_line_y = (dir.y > 0 ? (1 - b_y) : -b_y) + i_y* (dir.y > 0 ? 1 : -1);
		t_vector intersection_y = get_intersection(slop_x, 0, 0, next_line_y);
		//intersection_y.x *= -1;

		//double next_line_x =  floor(pos.x + ((dir.x < 0 && fmod(abs(pos.x + b_x), 1.0) == 0) ? -0.01 : 0) + 0) + (dir.x < 0? 0 : 1) - b_x;
		double next_line_x = (dir.x > 0 ? (1 - b_x) : -b_x) + i_x * (dir.x > 0 ? 1 : -1) ;
		t_vector intersection_x = get_intersection(slop_y, 0, 0, next_line_x);
		intersection_x = new_vector(intersection_x.y, intersection_x.x);

		double magnetude_x = vector_get_magnetude(new_vector(intersection_x.x - pos.x, intersection_x.y - pos.y));
		double magnetude_y = vector_get_magnetude(new_vector(intersection_y.x - pos.x, intersection_y.y - pos.y));
		//printf("pos: %f %f, y: %f, x: %f, next_x: %f, next_y: %f, ix: %d, iy %d\n", pos.x, pos.y, b_y , b_x, next_line_x, next_line_y, i_x, i_y);
		//t_vector intersection = intersection_y;
		t_vector intersection = (magnetude_y > magnetude_x) ? intersection_x : intersection_y;
		//render_dot(vars, (int)floor((intersection.x + origin.x) * (double)WIN_WIDTH / (double)vars->width_m), (int)floor((intersection.y + origin.y) * (double)WIN_HEIGHT / (double)vars->height_m), 0xff0000);
		//printf("x: %f y: %f, pos: %f %f,  x: %f %f y:%f %f\n", magnetude_x, magnetude_y, pos.x, pos.y, intersection_x.x, intersection_x.y, intersection_y.x, intersection_y.y);

		if (magnetude_y > magnetude_x) i_x += 1;
		else i_y += 1;

		pos.x = intersection.x;
		pos.y = intersection.y;
		double colide = collide_face(vars, pos.x + origin.x, pos.y + origin.y);
		if (colide != -1)
		{
			raycast.hit_pos = new_vector(pos.x + origin.x, pos.y + origin.y);
			raycast.hit_dist = vector_get_magnetude(pos);
			raycast.x_hit = (magnetude_y > magnetude_x) ? fmod(raycast.hit_pos.y, 1) : fmod(raycast.hit_pos.x, 1);
			raycast.hit_color = colide;
			raycast.map_box_hit = get_map_cell_colide(vars, pos.x + origin.x, pos.y + origin.y, colide);
            printf("%f %f\n", raycast.map_box_hit.x, raycast.map_box_hit.y);
			break;
		}
	}

	//render_dot(vars, (int)(raycast.hit_pos.x * WIN_WIDTH / vars->width_m), (int)(raycast.hit_pos.y * WIN_HEIGHT / vars->height_m), 0x00ff00);
	return raycast;
}

double collide_face(t_vars *vars, double x, double y)
{
    if (y >= (vars->height_m - 0.01)) return WALL_COLOR_UP;
    if (y <= 0.01) return WALL_COLOR_DOWN;
    if (x >= (vars->width_m - 0.01)) return WALL_COLOR_LEFT;
    if (x <= 0.01) return WALL_COLOR_RIGHT;




	if ((fmod(y, 1) == 0 && get_map_value(vars, x, y, 0xF) == 0xF) || y >= vars->height_m) return WALL_COLOR_UP;
	if ((fmod(y, 1) == 0 && get_map_value(vars, x, y - 1, 0xF) == 0xF) || y <= 0) return WALL_COLOR_DOWN;
	if ((fmod(x, 1) == 0 && get_map_value(vars, x, y, 0xF) == 0xF)  || x >= vars->width_m) return WALL_COLOR_LEFT;
	if ((fmod(x, 1) == 0 && get_map_value(vars, x - 1, y, 0xF) == 0xF) || x <= 0) return WALL_COLOR_RIGHT;

	return -1;
}

t_vector get_map_cell_colide(t_vars *vars, double _x, double _y, double color)
{
    double x = _x;
    double y = _y;
    if (color == WALL_COLOR_UP)
    {
        y -= 0;
        x = (int)x;
    }
    if (color == WALL_COLOR_DOWN)
    {
        y -= 1;
        x = (int)x;
    }
    if (color == WALL_COLOR_RIGHT)
    {
        x -= 1;
        y = (int)y;
    }

    if (color == WALL_COLOR_LEFT)
    {
        x -= 0;
        y = (int)y;
    }

    return new_vector(x, y);
}

t_vector get_intersection(double a, double b, double a_prime, double b_prime)
{
	t_vector vector;
	vector.x = (b_prime - b) / (a - a_prime);
	vector.y = a_prime * vector.x + b_prime;
	return vector;
} 

