/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:56:24 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:05:55 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_sdl		*init(void)
{
	t_sdl *sdl;

	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		handle_error("Malloc failed");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error_sdl("SDL could not initialize!");
	sdl->window = SDL_CreateWindow(PROGRAM_TITLE, 100, 100,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error_sdl("Window could not be created!");
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error_sdl("SDL image could not initialize!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	if (!sdl->screen)
		handle_error_sdl("Surface could not be created!");
	if (TTF_Init() == -1)
		handle_error("SDL_ttf could not initialize!");
	if (!(sdl->font = TTF_OpenFont("fonts/Action_Man.ttf", 20)))
		handle_error_sdl("Failed to load font!");
	return (sdl);
}

void		set_spawn_point(t_player *player, t_map *map)
{
	int	col;
	int	row;

	col = 0;
	row = 0;
	while (row < map->rows)
	{
		col = 0;
		while (col < map->cols)
		{
			if (map->map[row][col] == SPAWN_POINT)
			{
				player->spawn_x = (double)col;
				player->spawn_y = (double)row;
				return ;
			}
			col++;
		}
		row++;
	}
	handle_error("No spawn point found in map.");
}

t_player	*init_player(t_map *map)
{
	t_player *player;

	if (!(player = (t_player*)ft_memalloc(sizeof(*player))))
		handle_error("Malloc failed");
	set_spawn_point(player, map);
	player->x = player->spawn_x + 0.5;
	player->y = player->spawn_y + 0.5;
	ft_printf("Player spawned at %f %f\n", player->x, player->y);
	player->dir_x = 1;
	player->dir_y = 0.000001;
	player->plane_x = 0;
	player->plane_y = 0.66;
	player->move_speed = MOVE_SPEED;
	player->rotation_speed = ROTATION_SPEED;
	player->vertical_fov = VERTICAL_FOV_DIV;
	return (player);
}

void		draw_loading_screen(t_sdl *sdl)
{
	int i;
	int	pixel_amount;
	int color;

	i = 0;
	pixel_amount = SCREEN_HEIGHT * SCREEN_WIDTH;
	color = 0xBBBBBB;
	while (i < pixel_amount)
	{
		put_pixel(sdl->screen, i % SCREEN_WIDTH, i / SCREEN_WIDTH, color);
		i++;
	}
	if (!sdl->text_surface)
		sdl->text_surface = TTF_RenderText_Solid(sdl->font, "Loading...",
			(SDL_Color){255, 255, 255, 0});
	SDL_BlitSurface(sdl->text_surface, NULL, sdl->screen,
		&(SDL_Rect){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50});
	sdl->time_prev = sdl->time_now;
}

void		open_textures(t_sdl *sdl)
{
	static int	i;
	static int	textures_amount;
	char		**textures;
	char		*texture_path;

	textures = ft_strsplit(TEXTURES, ' ');
	while (textures[textures_amount])
		textures_amount++;
	if ((textures_amount / 4) < sdl->map->max_num)
		handle_error("Invalid texture number in map");
	sdl->textures_amount = textures_amount ? textures_amount : -1;
	if (!(sdl->textures = (SDL_Surface**)ft_memalloc(sizeof(SDL_Surface*)
		* textures_amount)))
		handle_error("Malloc failed");
	while (i < textures_amount)
	{
		texture_path = ft_strjoin(TEXTURES_FOLDER, textures[i]);
		if (!(sdl->textures[i++] = IMG_Load(texture_path)))
			handle_error("Texture not found");
		free(texture_path);
	}
	i = 0;
	while (textures[i])
		free(textures[i++]);
	free(textures);
}
