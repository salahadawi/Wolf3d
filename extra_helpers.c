/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 19:05:31 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:06:28 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_fps(t_sdl *sdl)
{
	static int		i;
	static char		*text;

	if (i++ % 50 == 0)
	{
		if (sdl->text_surface)
			SDL_FreeSurface(sdl->text_surface);
		if (text)
			free(text);
		text = ft_sprintf("FPS: %.0f", CLOCKS_PER_SEC /
			(sdl->time_now - sdl->time_prev));
		sdl->text_surface = TTF_RenderText_Shaded(sdl->font, text,
		(SDL_Color){255, 255, 255, 0}, (SDL_Color){0, 0, 0, 0});
	}
	SDL_BlitSurface(sdl->text_surface, NULL, sdl->screen, NULL);
	sdl->time_prev = sdl->time_now;
}

void	handle_frame(t_sdl *sdl)
{
	update_player_speed(sdl);
	draw_background(sdl);
	draw_map(sdl);
	draw_minimap(sdl);
	draw_fps(sdl);
}

void	clear_surface(SDL_Surface *surface)
{
	int i;
	int	pixel_amount;

	i = 0;
	pixel_amount = SCREEN_HEIGHT * SCREEN_WIDTH;
	while (i < pixel_amount)
	{
		put_pixel(surface, i % SCREEN_WIDTH, i / SCREEN_WIDTH, 0);
		i++;
	}
}

void	get_map_box(t_sdl *sdl)
{
	sdl->math.map_x = (int)sdl->player->x;
	sdl->math.map_y = (int)sdl->player->y;
}
