#include "define.h"

void update_player(t_input *input, t_player *player, double deltat_ime)
{
	t_vector velocity =  new_vector(-(input->left_pressed) + input->right_pressed, -(input->up_pressed) + input->down_pressed);
	velocity = vector_get_normal(velocity);
	double velocity_magnetude = vector_get_magnetude(velocity);
	double angle_vel = acos(-velocity.y) * (velocity.x > 0 ? -1 : 1);
	//printf("\r %f %f  %f %f %f                    ", velocity.x, velocity.y, player->position.x, player->position.y, deltat_ime);
	player->angle = fmod(player->angle + -input->mouse_delta.x / 10 * SENSITIVITY, 360);
	set_vector(&player->position,
		player->position.x + sin(angle_vel + player->angle / DEGRE) * velocity_magnetude * SPEED * deltat_ime,
		player->position.y + cos(angle_vel + player->angle / DEGRE) * velocity_magnetude * SPEED * deltat_ime);

	//printf("\r%f %f %f %f", player->position.x, player->position.y, angle_vel * DEGRE, velocity_magnetude);
}

void init_player(t_vars *vars)
{
	vars->player = malloc(sizeof(t_player));
	vars->player->position = new_vector(4, 4);
	vars->player->angle = 90;
}