#include "wolf3d.h"

/*
** Takes as parameter the function to use, to either draw a certain color or
** modify an already existing pixel
*/

void	draw_box(SDL_Surface *screen, int xywh[4], int color,
					void (*f)(SDL_Surface*, int, int, int))
{
	int x;
	int y;
	int width;
	int height;

	x = xywh[0];
	y = xywh[1];
	height = xywh[3];
	while (height--)
	{
		width = xywh[2];
		while (width--)
			(*f)(screen, x + width, y + height, color);
	}
}

void	draw_minimap_map(t_sdl *sdl)
{
	int col;
	int row;
	int	blocks_visible;

	blocks_visible = 15;
	row = 0;
	while (row < sdl->map->rows)
	{
		col = 0;
		while (col < sdl->map->cols)
		{
			if (sdl->map->map[row][col] > 0)
				draw_box(sdl->screen,
				(int[4]){(col - sdl->player->x) * 210 / blocks_visible + 120,
				(row - sdl->player->y) * 210 / blocks_visible + 120,
				210 / blocks_visible,
				210 / blocks_visible},
				0x888888, &put_minimap_pixel);
			col++;
		}
		row++;
	}
}

void	draw_minimap_fov_cone(t_sdl *sdl)
{
	int		angle;
	double	rotation_x;
	double	rotation_y;
	double	old_rotation_x;

	rotation_x = sdl->player->dir_x;
	rotation_y = sdl->player->dir_y;
	angle = -60;
	while (angle++ < 0)
	{
		old_rotation_x = rotation_x;
		rotation_x = rotation_x * cos(-0.01) - rotation_y * sin(-0.01);
		rotation_y = old_rotation_x * sin(-0.01) + rotation_y * cos(-0.01);
	}
	while (angle++ < 120)
	{
		draw_line(sdl, (int[2]){120, (int)(rotation_x * 40) + 120},
			(int[2]){120, (int)(rotation_y * 40) + 120}, 0x001500);
		old_rotation_x = rotation_x;
		rotation_x = rotation_x * cos(0.01) - rotation_y * sin(0.01);
		rotation_y = old_rotation_x * sin(0.01) + rotation_y * cos(0.01);
	}
}

void	draw_minimap(t_sdl *sdl)
{
	draw_box(sdl->screen, (int[4]){25, 25, 220, 220}, 0x333333, &put_pixel);
	draw_box(sdl->screen, (int[4]){30, 30, 210, 210}, 0x999999,
		&modify_pixel_remove);
	draw_minimap_map(sdl);
	draw_minimap_fov_cone(sdl);
	draw_box(sdl->screen, (int[4]){115, 115, 10, 10}, 0xFF0000, &put_pixel);
}

void	put_minimap_pixel(SDL_Surface *screen, int x, int y, int color)
{
	int *pixel;

	if (x > 240 || y > 240 || x < 30 || y < 30)
		return ;
	pixel = screen->pixels + y * screen->pitch +
		x * screen->format->BytesPerPixel;
	*pixel = color;
}
