/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 17:52:44 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	get_map_box(t_sdl *sdl)
{
	sdl->math.map_x = (int)sdl->player->x;
	sdl->math.map_y = (int)sdl->player->y;
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

void	update_player_speed(t_sdl *sdl)
{
	sdl->time_prev = sdl->time_now;
	sdl->time_now = clock();
	sdl->player->move_speed = (sdl->time_now - sdl->time_prev) / 1000 / 150;
	if (sdl->player->crouching)
		sdl->player->move_speed *= CROUCH_MOVE_SPEED_MULT;
	sdl->player->rotation_speed = (sdl->time_now - sdl->time_prev) / 1000 / 250;
}

void	draw_loading_screen(t_sdl *sdl)
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

void	open_textures(t_sdl *sdl)
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

void	handle_player_movement(t_sdl *sdl)
{
	if (sdl->input.left || sdl->input.right)
		handle_player_turning(sdl);
	if (sdl->input.up || sdl->input.down)
		handle_player_walking(sdl);
	if (sdl->input.jump)
		player_jump(sdl->player, &sdl->input.jump);
	if (sdl->input.crouch)
		player_crouch(sdl->player, 1);
	else if (sdl->player->crouching)
		player_crouch(sdl->player, 0);
}

void	handle_keys_down(t_sdl *sdl)
{
	if (sdl->e.key.keysym.sym == SDLK_ESCAPE)
		close_sdl(sdl);
	if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
		sdl->input.left = 1;
	if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
		sdl->input.right = 1;
	if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
		sdl->input.up = 1;
	if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
		sdl->input.down = 1;
	if (sdl->e.key.keysym.sym == SDLK_SPACE)
		sdl->input.jump = 1;
	if (sdl->e.key.keysym.sym == SDLK_e)
		sdl->pixelation++;
	if (sdl->e.key.keysym.sym == SDLK_q)
		sdl->pixelation ? sdl->pixelation-- : (void)sdl->pixelation;
	if (sdl->e.key.keysym.sym == SDLK_LCTRL)
		sdl->input.crouch = 1;
	if (sdl->e.key.keysym.sym == SDLK_x && sdl->player->vertical_fov <= 4.0)
		sdl->player->vertical_fov += FOV_CHANGE_AMT;
	if (sdl->e.key.keysym.sym == SDLK_c && sdl->player->vertical_fov >= 1.0)
		sdl->player->vertical_fov -= FOV_CHANGE_AMT;
}

void	handle_keys_up(t_sdl *sdl)
{
	if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
		sdl->input.left = 0;
	if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
		sdl->input.right = 0;
	if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
		sdl->input.up = 0;
	if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
		sdl->input.down = 0;
	if (sdl->e.key.keysym.sym == SDLK_LCTRL)
		sdl->input.crouch = 0;
}

void	handle_frame(t_sdl *sdl)
{
	update_player_speed(sdl);
	draw_background(sdl);
	draw_map(sdl);
	draw_minimap(sdl);
	draw_fps(sdl);
}

void	play_music_in_separate_process(void)
{
	if (!fork())
	{
		system("afplay audio/bg_music.mp3");
		exit(0);
	}
}

void	handle_keys(t_sdl *sdl)
{
	if (SDL_PollEvent(&sdl->e))
	{
		if (sdl->e.type == SDL_QUIT)
			close_sdl(sdl);
		else if (sdl->e.type == SDL_KEYDOWN)
			handle_keys_down(sdl);
		else if (sdl->e.type == SDL_KEYUP)
			handle_keys_up(sdl);
	}
}

int		main(int argc, char **argv)
{
	t_sdl	*sdl;

	sdl = init();
	handle_arguments(sdl, argc, argv);
	sdl->player = init_player(sdl->map);
	print_map(sdl->map);
	open_textures(sdl);
	play_music_in_separate_process();
	while (1)
	{
		if (!sdl->loading_done)
		{
			draw_loading_screen(sdl);
			sdl->loading_done = 1;
			SDL_RaiseWindow(sdl->window);
		}
		else
			handle_frame(sdl);
		SDL_UpdateWindowSurface(sdl->window);
		handle_keys(sdl);
		handle_player_movement(sdl);
	}
	close_sdl(sdl);
}
