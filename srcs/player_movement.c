/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 17:34:04 by alcohen           #+#    #+#             */
/*   Updated: 2020/08/17 21:23:36 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	handle_jump_height(t_sdl *sdl)
{
	static int going_down;

	if (going_down)
	{
		if (sdl->player->jump_height < 50)
			sdl->player->jump_height -= 3;
		else
			sdl->player->jump_height -= 2;
	}
	else
	{
		if (sdl->player->jump_height < 50)
			sdl->player->jump_height += 3;
		else
			sdl->player->jump_height += 2;
	}
	if (sdl->player->jump_height > 100)
		going_down = 1;
	if (sdl->player->jump_height <= 0 && going_down)
	{
		sdl->player->jump_height = 0;
		sdl->input.jump = 0;
		going_down = 0;
	}
}
