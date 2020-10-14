/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 17:34:04 by alcohen           #+#    #+#             */
/*   Updated: 2020/10/13 17:46:02 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	player_jump(t_player *player, int *input_jump)
{
	static int going_down;

	if (going_down)
	{
		if (player->cam_height < 50)
			player->cam_height -= 3;
		else
			player->cam_height -= 2;
	}
	else
	{
		if (player->cam_height < 50)
			player->cam_height += 3;
		else
			player->cam_height += 2;
	}
	if (player->cam_height > 100)
		going_down = 1;
	if (player->cam_height <= 0 && going_down)
	{
		player->cam_height = 0;
		*input_jump = 0;
		going_down = 0;
	}
}

void	player_crouch(t_player *player, int crouch)
{
	if (crouch)
	{
		if (!player->busy)
		{
			player->busy = 1;
			player->cam_height = -CROUCH_AMT;
		}
		player->crouching = 1;
	}
	else
	{
		player->busy = 0;
		player->cam_height = 0;
		player->crouching = 0;
	}
}

void	player_walk_forward(t_player *player, t_map *map)
{
	if (player->y + player->dir_y *
			player->move_speed + (player->dir_y > 0 ? 0.3 : -0.3) <
			map->rows && player->y + player->dir_y * player->move_speed +
			(player->dir_y > 0 ? 0.3 : -0.3) > 0)
		if (map->map[(int)(player->y +
			player->dir_y * player->move_speed +
			(player->dir_y >
			0 ? 0.3 : -0.3))][(int)(player->x)] < 1)
			player->y += player->dir_y *
			player->move_speed;
	if (player->x + player->dir_x *
			player->move_speed + (player->dir_x > 0 ? 0.3 : -0.3) <
			map->cols && player->x + player->dir_x *
			player->move_speed + (player->dir_x >
			0 ? 0.3 : -0.3) > 0)
		if (map->map[(int)(player->y)][(int)(player->x
			+ player->dir_x * player->move_speed +
			(player->dir_x > 0 ? 0.3 : -0.3))] < 1)
			player->x += player->dir_x *
			player->move_speed;
}

void	player_walk_backward(t_player *player, t_map *map)
{
	if (player->y - player->dir_y *
			player->move_speed - (player->dir_y > 0 ? 0.3 : -0.3) <
			map->rows && player->y -
			player->dir_y * player->move_speed -
			(player->dir_y > 0 ? 0.3 : -0.3) > 0)
		if (map->map[(int)(player->y -
			player->dir_y * player->move_speed -
			(player->dir_y >
			0 ? 0.3 : -0.3))][(int)(player->x)] < 1)
			player->y -= player->dir_y *
			player->move_speed;
	if (player->x - player->dir_x *
		player->move_speed - (player->dir_x > 0 ? 0.3 : -0.3) <
			map->cols && player->x -
				player->dir_x * player->move_speed -
					(player->dir_x > 0 ? 0.3 : -0.3) > 0)
		if (map->map[(int)(player->y)][(int)(player->x
			- player->dir_x * player->move_speed -
				(player->dir_x > 0 ? 0.3 : -0.3))] < 1)
			player->x -= player->dir_x *
				player->move_speed;
}

void	handle_player_walking(t_sdl *sdl)
{
	if (sdl->input.up)
	{
		player_walk_forward(sdl->player, sdl->map);
	}
	if (sdl->input.down)
	{
		player_walk_backward(sdl->player, sdl->map);
	}
}
