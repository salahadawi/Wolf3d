#include "wolf3d.h"

void	calculate_ray_len(t_sdl *sdl)
{
	sdl->math.delta_dist_x = fabs(1 / sdl->math.ray_dir_x);
	sdl->math.delta_dist_y = fabs(1 / sdl->math.ray_dir_y);
}

void	calculate_ray_pos(t_sdl *sdl, int x)
{
	sdl->math.camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	sdl->math.ray_dir_x = sdl->player->dir_x +
							sdl->player->plane_x * sdl->math.camera_x;
	sdl->math.ray_dir_y = sdl->player->dir_y +
							sdl->player->plane_y * sdl->math.camera_x;
}

void	calculate_perpendicular_dist(t_sdl *sdl)
{
	if (sdl->math.side == 0)
		sdl->math.perpendicular_wall_dist = (sdl->math.map_x - sdl->player->x +
		(1 - sdl->math.step_x) / 2) / sdl->math.ray_dir_x;
	else
		sdl->math.perpendicular_wall_dist = (sdl->math.map_y - sdl->player->y +
		(1 - sdl->math.step_y) / 2) / sdl->math.ray_dir_y;
}

void	calculate_line_height(t_sdl *sdl)
{
	sdl->math.line_height = (int)(SCREEN_HEIGHT /
	sdl->math.perpendicular_wall_dist);
	sdl->math.draw_start = -sdl->math.line_height /
	sdl->player->vertical_fov + SCREEN_HEIGHT / 2;
	sdl->math.draw_end = sdl->math.line_height /
	sdl->player->vertical_fov + SCREEN_HEIGHT / 2;
	if (sdl->math.draw_end >= SCREEN_HEIGHT)
		sdl->math.draw_end = SCREEN_HEIGHT - 1;
}

void	calculate_texture_x(t_sdl *sdl)
{
	if (sdl->math.side == 0)
	{
		sdl->math.wall_side = 0;
		if (sdl->math.map_x - sdl->player->x > 0)
			sdl->math.wall_side = 1;
		sdl->math.wall_x = sdl->player->y + sdl->math.perpendicular_wall_dist *
		sdl->math.ray_dir_y;
	}
	if (sdl->math.side == 1)
	{
		sdl->math.wall_side = 2;
		if (sdl->math.map_y - sdl->player->y > 0)
			sdl->math.wall_side = 3;
		sdl->math.wall_x = sdl->player->x + sdl->math.perpendicular_wall_dist *
		sdl->math.ray_dir_x;
	}
	sdl->math.wall_x -= floor((sdl->math.wall_x));
}
