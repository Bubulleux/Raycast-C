#include "define.h"

void update_player(t_vars *vars, t_input *input, t_player *player, double deltat_ime)
{
	t_vector velocity =  new_vector(-(input->left_pressed) + input->right_pressed, -(input->up_pressed) + input->down_pressed);
	velocity = vector_get_normal(velocity);
	double velocity_magnetude = vector_get_magnetude(velocity);
	double angle_vel = acos(-velocity.y) * (velocity.x > 0 ? -1 : 1);
	//printf("\r %f %f  %f %f %f                    ", velocity.x, velocity.y, player->position.x, player->position.y, deltat_ime);
	player->angle_x = fmod(player->angle_x + -input->mouse_delta.x / 10 * SENSITIVITY, 360);
	player->angle_x = player->angle_x < 0 ? 360 - player->angle_x : player->angle_x;

    player->angle_y = player->angle_y + input->mouse_delta.y / 10 * SENSITIVITY;
    if (player->angle_y > 90) player->angle_y = 90;
    if (player->angle_y < -90) player->angle_y = -90;
    //printf("\r%f    ", player->angle_y);

	t_vector futur_pos = new_vector(player->position.x + sin(angle_vel + player->angle_x / DEGRE) * velocity_magnetude * SPEED * deltat_ime,
                                    player->position.y + cos(angle_vel + player->angle_x / DEGRE) * velocity_magnetude * SPEED * deltat_ime);
	if (!check_collide(vars, futur_pos, COLIDE_BOX_SIZE))
    {
	    player->position = futur_pos;
    }
//	set_vector(&player->position,
//		player->position.x + sin(angle_vel + player->angle_x / DEGRE) * velocity_magnetude * SPEED * deltat_ime,
//		player->position.y + cos(angle_vel + player->angle_x / DEGRE) * velocity_magnetude * SPEED * deltat_ime);

	//printf("\r%f %f %f %f", player->position.x, player->position.y, angle_vel * DEGRE, velocity_magnetude);
    //printf("\r%f      ", player->angle_x);
}

void init_player(t_vars *vars)
{
	vars->player = malloc(sizeof(t_player));
	vars->player->position = new_vector(4, 4);
	vars->player->angle_x = 90;
    vars->player->angle_y = 0;
}

bool check_collide(t_vars *vars, t_vector pos, double box_size)
{
    return  get_map_value(vars, pos.x + box_size, pos.y + box_size, 0x0F) == 0x0F ||
//            get_map_value(vars, pos.x - box_size, pos.y + box_size, 0x0F) == 0x0F ||
//            get_map_value(vars, pos.x + box_size, pos.y - box_size, 0x0F) == 0x0F ||
            get_map_value(vars, pos.x - box_size, pos.y - box_size, 0x0F) == 0x0F;
}