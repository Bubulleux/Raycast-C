#include "define.h"

void make_raycast(t_vars *vars, double grid_size)
{
	t_vector mouse_pos;
	int x;
	int y;
	mlx_mouse_get_pos(vars->render->mlx, vars->render->window, &x, &y);
	mouse_pos.x = x - 400;
	mouse_pos.y = y - 400;
	t_vector mouse_normal = vector_get_normal(mouse_pos);
	t_vector pos = new_vector(0, 0);


	if (mouse_normal.y == 0 || mouse_normal.x == 0) return;

	double slop_x = mouse_normal.y / mouse_normal.x;
	double slop_y = mouse_normal.x / mouse_normal.y;

	for (int i; i < 10; i++)
	{
		double next_line_y =  floor((pos.y + (mouse_normal.y < 0 ? -1 : 0)) / grid_size) * grid_size + (mouse_normal.y < 0? 0 : grid_size);
		t_vector intersection_y = get_intersection(slop_x, 0, 0, next_line_y);
		//intersection_y.x *= -1;

		double next_line_x =  floor((pos.x + (mouse_normal.x < 0 ? -1 : 0)) / grid_size) * grid_size + (mouse_normal.x < 0? 0 : grid_size);
		t_vector intersection_x = get_intersection(slop_y, 0, 0, next_line_x);
		intersection_x = new_vector(intersection_x.y, intersection_x.x);

		//t_vector intersection = intersection_x;
		t_vector intersection = (vector_get_magnetude(intersection_y) > vector_get_magnetude(intersection_x) ? intersection_x : intersection_y);
		render_dot(vars, (int)floor(intersection.x) + 400, (int)floor(intersection.y) + 400, 0x00ff0000);
		pos.x = intersection.x;
		pos.y = intersection.y;
	}
}

t_vector get_intersection(double a, double b, double a_prime, double b_prime)
{
	t_vector vector;
	vector.x = (b_prime - b) / (a - a_prime);
	vector.y = a_prime * vector.x + b_prime;
	return vector;
} 
