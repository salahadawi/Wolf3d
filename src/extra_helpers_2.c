/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_helpers_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 19:08:38 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:08:39 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	play_music_in_separate_process(void)
{
	if (!fork())
	{
		system("afplay audio/bg_music.mp3");
		exit(0);
	}
}

void	update_player_speed(t_sdl *sdl)
{
	sdl->time_prev = sdl->time_now;
	sdl->time_now = clock();
	sdl->player->move_speed = (sdl->time_now - sdl->time_prev) / 1000 / 150;
	if (sdl->player->crouching)
		sdl->player->move_speed *= CROUCH_MOVE_SPEED_MULT;
	sdl->player->rotation_speed = (sdl->time_now - sdl->time_prev) / 1000 / 250;
}
