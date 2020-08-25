/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/21 17:12:30 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/21 18:23:35 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	plot_line_low(t_sdl *sdl, int x[2], int y[2], int color)
{
	int *dxy;
	int *xy;
	int yi;
	int d;

	dxy = (int[2]){x[1] - x[0], y[1] - y[0]};
	yi = 1;
	if (dxy[1] < 0)
	{
		yi = -1;
		dxy[1] *= -1;
	}
	d = 2 * dxy[1] - dxy[0];
	xy = (int[2]){x[0], y[0]};
	while (xy[0] <= x[1])
	{
		modify_pixel_add(sdl->screen, xy[0], xy[1], color);
		if (d > 0)
		{
			xy[1] += yi;
			d -= 2 * dxy[0];
		}
		xy[0]++;
		d += 2 * dxy[1];
	}
}

void	plot_line_high(t_sdl *sdl, int x[2], int y[2], int color)
{
	int *dxy;
	int *xy;
	int xi;
	int d;

	dxy = (int[2]){x[1] - x[0], y[1] - y[0]};
	xi = 1;
	if (dxy[0] < 0)
	{
		xi = -1;
		dxy[0] *= -1;
	}
	d = 2 * dxy[0] - dxy[1];
	xy = (int[2]){x[0], y[0]};
	while (xy[1] <= y[1])
	{
		modify_pixel_add(sdl->screen, xy[0], xy[1], color);
		if (d > 0)
		{
			xy[0] += xi;
			d -= 2 * dxy[1];
		}
		xy[1]++;
		d += 2 * dxy[0];
	}
}

void	swap_points(int *x[2], int *y[2])
{
	int tmp;

	tmp = (*x)[0];
	(*x)[0] = (*x)[1];
	(*x)[1] = tmp;
	tmp = (*y)[0];
	(*y)[0] = (*y)[1];
	(*y)[1] = tmp;
}

void	draw_line(t_sdl *sdl, int x[2], int y[2], int color)
{
	if (abs(y[1] - y[0]) < abs(x[1] - x[0]))
	{
		if (x[0] > x[1])
			swap_points(&x, &y);
		plot_line_low(sdl, x, y, color);
	}
	else
	{
		if (y[0] > y[1])
			swap_points(&x, &y);
		plot_line_high(sdl, x, y, color);
	}
}
