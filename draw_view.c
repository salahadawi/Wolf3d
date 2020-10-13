/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 19:05:23 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:05:32 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_map(t_sdl *sdl)
{
	int x;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		calculate_ray_pos(sdl, x);
		get_map_box(sdl);
		calculate_ray_len(sdl);
		calculate_side_dist(sdl);
		find_nearest_wall(sdl);
		calculate_perpendicular_dist(sdl);
		calculate_line_height(sdl);
		calculate_texture_x(sdl);
		draw_x(sdl, x);
		x += sdl->pixelation + 1;
	}
}

void	draw_background(t_sdl *sdl)
{
	int i;
	int	pixel_amount;
	int color;

	i = 0;
	pixel_amount = SCREEN_HEIGHT * SCREEN_WIDTH;
	color = 0x090909;
	while (i < pixel_amount / 2 + sdl->player->cam_height * SCREEN_WIDTH)
	{
		put_pixel(sdl->screen, i % SCREEN_WIDTH, i / SCREEN_WIDTH,
		(i / 100) ? color : color / 2);
		i++;
	}
	color = 0x333333;
	while (i < pixel_amount)
	{
		put_pixel(sdl->screen, i % SCREEN_WIDTH, i / SCREEN_WIDTH, color);
		i++;
	}
}
