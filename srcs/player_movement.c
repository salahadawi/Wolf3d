/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 17:34:04 by alcohen           #+#    #+#             */
/*   Updated: 2020/08/20 13:53:14 by alcohen          ###   ########.fr       */
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
		player->cam_height = -CROUCH_AMT;
		player->crouching = 1;
	}
	else
	{
		player->cam_height = 0;
		player->crouching = 0;
	} 
}