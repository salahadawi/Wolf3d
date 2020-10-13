#include "wolf3d.h"


void	player_turn_right(t_player *player, double old_dir_x,
							double old_plane_x)
{
	player->dir_x = player->dir_x *
		cos(-player->rotation_speed) -
			player->dir_y * sin(-player->rotation_speed);
	player->dir_y = old_dir_x *
		sin(-player->rotation_speed) +
			player->dir_y * cos(-player->rotation_speed);
	player->plane_x = player->plane_x *
		cos(-player->rotation_speed) -
			player->plane_y * sin(-player->rotation_speed);
	player->plane_y = old_plane_x *
		sin(-player->rotation_speed) +
			player->plane_y * cos(-player->rotation_speed);
}

void	player_turn_left(t_player *player, double old_dir_x,
							double old_plane_x)
{
	player->dir_x = player->dir_x *
		cos(player->rotation_speed) -
			player->dir_y * sin(player->rotation_speed);
	player->dir_y = old_dir_x *
		sin(player->rotation_speed) +
			player->dir_y * cos(player->rotation_speed);
	player->plane_x = player->plane_x *
		cos(player->rotation_speed) -
			player->plane_y * sin(player->rotation_speed);
	player->plane_y = old_plane_x *
		sin(player->rotation_speed) +
			player->plane_y * cos(player->rotation_speed);
}

void	handle_player_turning(t_sdl *sdl)
{
	double old_dir_x;
	double old_plane_x;

	old_dir_x = sdl->player->dir_x;
	old_plane_x = sdl->player->plane_x;
	if (sdl->input.right && !sdl->input.left)
	{
		player_turn_right(sdl->player, old_dir_x, old_plane_x);
	}
	else if (sdl->input.left && !sdl->input.right)
	{
		player_turn_left(sdl->player, old_dir_x, old_plane_x);
	}
}
