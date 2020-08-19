/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_pixels.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/19 16:43:08 by alcohen           #+#    #+#             */
/*   Updated: 2020/08/19 18:49:09 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

//Add the specified color to whatever color is already in the pixel

void modify_pixel_add(SDL_Surface *screen, int x, int y, int color)
{
	int *pixel;
	int	red;
	int	green;
	int	blue;

	(void)color;
	pixel = screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
	red = (*pixel / 256 / 256 % 256) + (color / 256 / 256 % 256);
	green = (*pixel / 256 % 256) + (color / 256 % 256);
	blue = (*pixel % 256) + (color % 256);
	red = red > 255 ? 255 : red;
	green = green > 255 ? 255 : green;
	blue = blue > 255 ? 255 : blue;
	*pixel = blue + green * 256 + red * 256 * 256;
}

//Remove the specified color from whatever color is already in the pixel

void modify_pixel_remove(SDL_Surface *screen, int x, int y, int color)
{
	int *pixel;
	int	red;
	int	green;
	int	blue;

	(void)color;
	pixel = screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
	red = (*pixel / 256 / 256 % 256) - (color / 256 / 256 % 256);
	green = (*pixel / 256 % 256) - (color / 256 % 256);
	blue = (*pixel % 256) - (color % 256);
	red = red < 0 ? 0 : red;
	green = green < 0 ? 0 : green;
	blue = blue < 0 ? 0 : blue;
	*pixel = blue + green * 256 + red * 256 * 256;
}

void	add_fog_to_pixel(SDL_Surface *screen, int x, int y, double wall_dist)
{
	double	intensity;

	if (x > SCREEN_WIDTH - 1 || y > SCREEN_HEIGHT - 1 || x < 0 || y < 0)
		return;
	intensity = (1.0 / (wall_dist)) * LIGHTING_INTENSITY;
	modify_pixel_multiply(screen, x, y, intensity);
}

void	modify_pixel_multiply(SDL_Surface *screen, int x, int y, double amount)
{
	int *pixel;
	int	red;
	int	green;
	int	blue;

	pixel = screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
	red = (*pixel / 256 / 256 % 256);
	green = (*pixel / 256 % 256);
	blue = (*pixel % 256);
	blue *= amount;
	red *= amount;
	green *= amount;
	red = red > 255 ? 255 : red;
	green = green > 255 ? 255 : green;
	blue = blue > 255 ? 255 : blue;
	*pixel = blue + green * 256 + red * 256 * 256;
}
