/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:56:37 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 18:56:38 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		get_pixel(SDL_Surface *screen, int x, int y)
{
	int		*pixel;
	Uint8	red;
	Uint8	green;
	Uint8	blue;
	Uint8	bpp;

	if (x > SCREEN_WIDTH - 1 || y > SCREEN_HEIGHT - 1)
		return (0);
	bpp = screen->format->BytesPerPixel;
	pixel = screen->pixels + y * screen->pitch + x * bpp;
	if (bpp == 1)
		SDL_GetRGB((Uint8)*pixel, screen->format, &red, &green, &blue);
	else if (bpp == 2)
		SDL_GetRGB((Uint16) * pixel, screen->format, &red, &green, &blue);
	else
		SDL_GetRGB(*pixel, screen->format, &red, &green, &blue);
	return (red * 256 * 256 + green * 256 + blue);
}

void	put_pixel(SDL_Surface *screen, int x, int y, int color)
{
	int *pixel;

	if (x > SCREEN_WIDTH - 1 || y > SCREEN_HEIGHT - 1 || x < 0 || y < 0)
		return ;
	pixel = screen->pixels + y * screen->pitch +
		x * screen->format->BytesPerPixel;
	*pixel = color;
}
