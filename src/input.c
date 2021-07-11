#include "define.h"

int key_pressed(int key, t_input *input)
{
    //printf("keyPressed: %d\n", key);
	if (key == UP_ARROW) input->up_pressed = 1;
	if (key == DOWN_ARROW) input->down_pressed = 1;
	if (key == LEFT_ARROW) input->left_pressed = 1;
	if (key == RIGHT_ARROW) input->right_pressed = 1;
    if (key == MINI_MAP_INPUT) input->enter_pressed = 1;
    if (key == ESCAPE_BTN)
    {
        input->lock_mouse = 0x00;
    }
}

int key_relased(int key, t_input *input)
{
	if (key == UP_ARROW) input->up_pressed = 0;
	if (key == DOWN_ARROW) input->down_pressed = 0;
	if (key == LEFT_ARROW) input->left_pressed = 0;
	if (key == RIGHT_ARROW) input->right_pressed = 0;
    if (key == MINI_MAP_INPUT) input->enter_pressed = 0;
}



void init_input(t_vars *vars)
{
	vars->input = malloc(sizeof(t_input));
	mlx_hook(vars->render->window_3D, 2, 1L<<0, key_pressed, vars->input);
	mlx_hook(vars->render->window_3D, 3, 1L<<1, key_relased, vars->input);
	vars->input->up_pressed = 0;
	vars->input->down_pressed = 0;
	vars->input->right_pressed = 0;
	vars->input->left_pressed = 0;
	vars->input->enter_pressed = 0;
	vars->input->lock_mouse = 0xF;
    mlx_mouse_hide(vars->render->mlx, vars->render->window_3D);
}

void update_mouse(t_input *input, t_vars *vars)
{
	int x;
	int y;
	mlx_mouse_get_pos(vars->render->mlx, vars->render->window_3D, &x, &y);
	set_vector(&input->mouse_delta, x - input->mouse_pos.x, y - input->mouse_pos.y);
	if (input->lock_mouse == 0xF)
    {
        mlx_mouse_move(vars->render->mlx, vars->render->window_3D, WIN_WIDTH / 2, WIN_HEIGHT / 2);
        set_vector(&input->mouse_pos, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
	else
    {
        set_vector(&input->mouse_pos, x, y);
        mlx_mouse_show(vars->render->mlx, vars->render->window_3D);
    }
}