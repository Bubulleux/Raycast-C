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
		t_vector dote = make_raycast(vars, new_vector(4, 4), new_vector(sin((angle_dir + i) / DEGRE), cos((angle_dir + i) / DEGRE)));
		render_dot(vars, (int)(dote.x * WIN_WIDTH / 8), (int)(dote.y * WIN_HEIGHT / 8), 0x00ff00);
	}
}

t_vector make_raycast(t_vars *vars, t_vector origin, t_vector dir)
{
	
	t_vector pos = new_vector(0, 0);

	if (dir.y == 0 || dir.x == 0) return new_vector(0, 0);

	double slop_x = dir.y / dir.x;
	double slop_y = dir.x / dir.y;

	for (int i; i < 10; i++)
	{
		double next_line_y =  floor((pos.y + (dir.y < 0 ? -1 : 0))) + (dir.y < 0? 0 : 1);
		t_vector intersection_y = get_intersection(slop_x, 0, 0, next_line_y);
		//intersection_y.x *= -1;

		double next_line_x =  floor((pos.x + (dir.x < 0 ? -1 : 0))) + (dir.x < 0? 0 : 1);
		t_vector intersection_x = get_intersection(slop_y, 0, 0, next_line_x);
		intersection_x = new_vector(intersection_x.y, intersection_x.x);

		//t_vector intersection = intersection_x;
		t_vector intersection = (vector_get_magnetude(intersection_y) > vector_get_magnetude(intersection_x) ? intersection_x : intersection_y);
		render_dot(vars, (int)floor(intersection.x * WIN_WIDTH / 8) + 400, (int)floor(intersection.y * WIN_HEIGHT / 8) + 400, 0xff0000);
		pos.x = intersection.x;
		pos.y = intersection.y;
		if (collide_face(vars, pos.x + origin.x, pos.y + origin.y))
		{
			return new_vector(pos.x + origin.x, pos.y + origin.y);
		}
	}

	return new_vector(pos.x + origin.x, pos.y + origin.y);
}

bool collide_face(t_vars *vars, double x, double y)
{
	if ((fmod(x, 1) == 0 && (get_map_value(vars->map, x, y) == 0xF || get_map_value(vars->map, x - 1, y) == 0xF)) ||
		(fmod(y, 1) == 0 && (get_map_value(vars->map, x, y) == 0xF || get_map_value(vars->map, x, y - 1) == 0xF)) ||
		x <= 0 || y <= 0 || x >= 8 || y >= 8)
	{
			return true;
	}
	return false;
}

t_vector get_intersection(double a, double b, double a_prime, double b_prime)
{
	t_vector vector;
	vector.x = (b_prime - b) / (a - a_prime);
	vector.y = a_prime * vector.x + b_prime;
	return vector;
} 
