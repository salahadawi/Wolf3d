/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:56:08 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 18:56:18 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_vertical_line_from_image(t_sdl *sdl, SDL_Surface *texture,
										int x[2], int y[2])
{
	int		tex_y;
	double	step;
	double	tex_pos;

	step = 1.0 * texture->h / (y[1] - y[0]);
	tex_pos = (y[0] - SCREEN_HEIGHT / 2 + (y[1] - y[0]) / 2) * step;
	if (y[0] < 0 - sdl->player->cam_height)
	{
		tex_pos = tex_pos + step * abs(y[0] + sdl->player->cam_height);
		y[0] = 0 - sdl->player->cam_height;
	}
	if (y[1] > SCREEN_HEIGHT)
		y[1] = SCREEN_HEIGHT;
	while (y[0] < y[1])
	{
		tex_y = (int)tex_pos & (texture->h - 1);
		put_pixel(sdl->screen, x[0], y[0] + sdl->player->cam_height,
				get_pixel(texture, x[1], tex_y));
		add_fog_to_pixel(sdl->screen, x[0], y[0] + sdl->player->cam_height,
				sdl->wall_dist);
		tex_pos += step;
		y[0]++;
	}
}

void	draw_x(t_sdl *sdl, int x)
{
	if (sdl->math.wall_side >= sdl->textures_amount)
		sdl->math.hit = 0;
	if (!sdl->math.hit)
		draw_out_of_bounds_line(sdl, x);
	else
		draw_map_line(sdl, x);
}

void	draw_out_of_bounds_line(t_sdl *sdl, int x)
{
	int i;

	i = 0;
	while (i <= sdl->pixelation)
		draw_vertical_line(sdl, x + i++,
							(int[2]){sdl->math.draw_start, sdl->math.draw_end},
							OUT_OF_BOUNDS_COLOR);
}

void	draw_map_line(t_sdl *sdl, int x)
{
	int i;

	sdl->math.tex_idx = sdl->math.wall_side +
					(sdl->map->map[sdl->math.map_y][sdl->math.map_x] - 1) * 4;
	sdl->math.texture_x = (int)(sdl->math.wall_x *
								(double)sdl->textures[sdl->math.tex_idx]->w);
	sdl->math.texture_x = sdl->textures[sdl->math.tex_idx]->w
							- sdl->math.texture_x - 1;
	sdl->wall_dist = sdl->math.perpendicular_wall_dist;
	i = 0;
	while (i <= sdl->pixelation)
		draw_vertical_line_from_image(sdl, sdl->textures[sdl->math.tex_idx],
		(int[2]){x + i++, sdl->math.texture_x}, (int[2]){sdl->math.draw_start,
		sdl->math.draw_end});
}

void	draw_vertical_line(t_sdl *sdl, int x, int y[2], int color)
{
	(void)color;
	y[0] += sdl->player->cam_height;
	y[1] += sdl->player->cam_height;
	while (y[0] < y[1])
		modify_pixel_add(sdl->screen, x, y[0]++, OUT_OF_BOUNDS_COLOR);
}
