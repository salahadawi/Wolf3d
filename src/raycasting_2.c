/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 19:08:43 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:08:44 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	find_nearest_wall(t_sdl *sdl)
{
	sdl->math.hit = 0;
	while (sdl->math.hit == 0)
	{
		if (sdl->math.side_dist_x < sdl->math.side_dist_y)
		{
			sdl->math.side_dist_x += sdl->math.delta_dist_x;
			sdl->math.map_x += sdl->math.step_x;
			sdl->math.side = 0;
		}
		else
		{
			sdl->math.side_dist_y += sdl->math.delta_dist_y;
			sdl->math.map_y += sdl->math.step_y;
			sdl->math.side = 1;
		}
		if (sdl->math.map_y >= sdl->map->rows ||
			sdl->math.map_x >= sdl->map->cols ||
			sdl->math.map_y < 0 || sdl->math.map_x < 0)
			break ;
		if (sdl->map->map[sdl->math.map_y][sdl->math.map_x] > 0)
			sdl->math.hit = 1;
	}
}

void	calculate_side_dist(t_sdl *sdl)
{
	if (sdl->math.ray_dir_x < 0)
	{
		sdl->math.step_x = -STEP_DIST_X;
		sdl->math.side_dist_x = (sdl->player->x - sdl->math.map_x) *
									sdl->math.delta_dist_x;
	}
	else
	{
		sdl->math.step_x = STEP_DIST_X;
		sdl->math.side_dist_x = (sdl->math.map_x + 1.0 - sdl->player->x) *
									sdl->math.delta_dist_x;
	}
	if (sdl->math.ray_dir_y < 0)
	{
		sdl->math.step_y = -STEP_DIST_Y;
		sdl->math.side_dist_y = (sdl->player->y - sdl->math.map_y) *
									sdl->math.delta_dist_y;
	}
	else
	{
		sdl->math.step_y = STEP_DIST_Y;
		sdl->math.side_dist_y = (sdl->math.map_y + 1.0 - sdl->player->y) *
									sdl->math.delta_dist_y;
	}
}
