#include "define.h"

void update_player(t_input *input, t_player *player, double deltat_ime)
{
	t_vector velocity =  new_vector(-(input->left_pressed) + input->right_pressed, -(input->up_pressed) + input->down_pressed);
	//printf("\r %f %f  %f %f %f                    ", velocity.x, velocity.y, player->position.x, player->position.y, deltat_ime);
	if (velocity.x == 0 && velocity.y == 0) return;

	//velocity = vector_get_normal(velocity);
	player->position = new_vector(player->position.x + velocity.x * SPEED * deltat_ime, player->position.y + velocity.y * SPEED * deltat_ime);
}

void init_player(t_vars *vars)
{
	vars->player = malloc(sizeof(t_player));
	vars->player->position = new_vector(4, 4);
}