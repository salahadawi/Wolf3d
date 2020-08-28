/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/28 15:12:30 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	close_sdl(t_sdl *sdl)
{
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void	handle_error_sdl(char *message)
{
	ft_printf("SDL Error: %s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void	handle_error(char *message)
{
	ft_printf("Error: %s\n", message);
	exit(0);
}

t_sdl	*init(void)
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
	player->posX = player->spawn_x + 0.5;
	player->posY = player->spawn_y + 0.5;
	ft_printf("Player spawned at %f %f\n", player->posX, player->posY);
	player->dirX = 1;
	player->dirY = 0.000001;
	player->planeX = 0;
	player->planeY = 0.66;
	player->move_speed = MOVE_SPEED;
	player->rotation_speed = ROTATION_SPEED;
	player->vertical_fov = VERTICAL_FOV_DIV;
	return (player);
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

void	put_minimap_pixel(SDL_Surface *screen, int x, int y, int color)
{
	int *pixel;

	if (x > 240 || y > 240 || x < 30 || y < 30)
		return ;
	pixel = screen->pixels + y * screen->pitch +
		x * screen->format->BytesPerPixel;
	*pixel = color;
}

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

void	draw_vertical_line_from_image(t_sdl *sdl, SDL_Surface *texture,
int x[2], int y[2])
{
	int tex_y;
	double step;
	double tex_pos;

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

void draw_vertical_line(t_sdl *sdl, int x, int y[2], int color)
{
	(void)color;
	y[0] += sdl->player->cam_height;
	y[1] += sdl->player->cam_height;
	while (y[0] < y[1])
		modify_pixel_add(sdl->screen, x, y[0]++, OUT_OF_BOUNDS_COLOR);
}

/*
** Fix norm in this function once we completely understand how it works
*/

void draw_map(t_sdl *sdl)
{
	int wall_side;
	for (int x = 0; x < SCREEN_WIDTH; x += sdl->pixelation + 1)
	{
		//calculate ray position and direction
		double cameraX = 2 * x / (double)SCREEN_WIDTH - 1; //x-coordinate in camera space
		double rayDirX = sdl->player->dirX + sdl->player->planeX * cameraX;
		double rayDirY = sdl->player->dirY + sdl->player->planeY * cameraX;
		//which box of the map we're in
		int mapX = (int)sdl->player->posX;
		int mapY = (int)sdl->player->posY;

		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		int hit = 0; //was there a wall hit?
		int side;	 //was a NS or a EW wall hit?
		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -STEP_DIST_X;
			sideDistX = (sdl->player->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = STEP_DIST_X;
			sideDistX = (mapX + 1.0 - sdl->player->posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -STEP_DIST_Y;
			sideDistY = (sdl->player->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = STEP_DIST_Y;
			sideDistY = (mapY + 1.0 - sdl->player->posY) * deltaDistY;
		}
		//perform DDA

		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray is out of bounds, if yes then exit loop
			if (mapY >= sdl->map->rows || mapX >= sdl->map->cols ||
				mapY < 0 || mapX < 0)
				break;
			//Check if ray has hit a wall
			if (sdl->map->map[mapY][mapX] > 0)
				hit = 1;
		}
		//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
		if (side == 0)
			perpWallDist = (mapX - sdl->player->posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - sdl->player->posY + (1 - stepY) / 2) / rayDirY;

		//Calculate height of line to draw on screen
		int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / sdl->player->vertical_fov + SCREEN_HEIGHT / 2;
		//if(drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / sdl->player->vertical_fov + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT)
			drawEnd = SCREEN_HEIGHT - 1;

		//choose wall color
		int color;
		//give x and y sides different brightness
		double wallX; //where exactly the wall was hit
		if (side == 0)
		{
			wall_side = 0;
			if (mapX - sdl->player->posX > 0)
				wall_side = 1;
			wallX = sdl->player->posY + perpWallDist * rayDirY;
		}
		if (side == 1)
		{
			wall_side = 2;
			if (mapY - sdl->player->posY > 0)
				wall_side = 3;
			wallX = sdl->player->posX + perpWallDist * rayDirX;
		}
		wallX -= floor((wallX));

		if (wall_side >= sdl->textures_amount)
			hit = 0;
		if (!hit)
		{
			color = 0xFF0000;
			for (int i = 0; i <= sdl->pixelation; i++)
				draw_vertical_line(sdl, + x + i, (int[2]){drawStart, drawEnd}, color);
		}
		else
		{
			//x coordinate on the texture
			int tex_idx = wall_side + (sdl->map->map[mapY][mapX] - 1) * 4;
			int texX = (int)(wallX * (double)sdl->textures[tex_idx]->w);

			texX = sdl->textures[tex_idx]->w - texX - 1;
			//draw the pixels of the stripe as a vertical line
			//verLine(x, drawStart, drawEnd, color);
			color = get_pixel(sdl->textures[0], texX, 50);
			sdl->wall_dist = perpWallDist;
			for (int i = 0; i <= sdl->pixelation; i++)
				draw_vertical_line_from_image(sdl, sdl->textures[tex_idx], (int[2]){x + i, texX}, (int[2]){drawStart, drawEnd});
		}
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
				(int[4]){(col - sdl->player->posX) * 210 / blocks_visible + 120,
				(row - sdl->player->posY) * 210 / blocks_visible + 120,
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

	rotation_x = sdl->player->dirX;
	rotation_y = sdl->player->dirY;
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
	int		i;
	int		textures_amount;
	char	**textures;
	char	*texture_path;

	textures = ft_strsplit(TEXTURES, ' ');
	textures_amount = 0;
	while (textures[textures_amount])
		textures_amount++;
	sdl->textures_amount = textures_amount ? textures_amount : -1;
	if (!(sdl->textures = (SDL_Surface**)ft_memalloc(sizeof(SDL_Surface*)
		* textures_amount)))
		handle_error("Malloc failed");
	i = 0;
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

void	handle_player_turning(t_sdl *sdl)
{
	double old_dir_x;
	double old_plane_x;

	old_dir_x = sdl->player->dirX;
	old_plane_x = sdl->player->planeX;
	if (sdl->input.right && !sdl->input.left)
	{
		sdl->player->dirX = sdl->player->dirX *
			cos(-sdl->player->rotation_speed) -
				sdl->player->dirY * sin(-sdl->player->rotation_speed);
		sdl->player->dirY = old_dir_x *
			sin(-sdl->player->rotation_speed) +
				sdl->player->dirY * cos(-sdl->player->rotation_speed);
		sdl->player->planeX = sdl->player->planeX *
			cos(-sdl->player->rotation_speed) -
				sdl->player->planeY * sin(-sdl->player->rotation_speed);
		sdl->player->planeY = old_plane_x *
			sin(-sdl->player->rotation_speed) +
				sdl->player->planeY * cos(-sdl->player->rotation_speed);
	}
	else if (sdl->input.left && !sdl->input.right)
	{
		sdl->player->dirX = sdl->player->dirX *
			cos(sdl->player->rotation_speed) -
				sdl->player->dirY * sin(sdl->player->rotation_speed);
		sdl->player->dirY = old_dir_x *
			sin(sdl->player->rotation_speed) +
				sdl->player->dirY * cos(sdl->player->rotation_speed);
		sdl->player->planeX = sdl->player->planeX *
			cos(sdl->player->rotation_speed) -
				sdl->player->planeY * sin(sdl->player->rotation_speed);
		sdl->player->planeY = old_plane_x *
			sin(sdl->player->rotation_speed) +
				sdl->player->planeY * cos(sdl->player->rotation_speed);
	}
}

void	handle_player_walking(t_sdl *sdl)
{
	if (sdl->input.up)
	{
		if (sdl->player->posY + sdl->player->dirY *
			sdl->player->move_speed + (sdl->player->dirY > 0 ? 0.3 : -0.3) <
				sdl->map->rows && sdl->player->posY +
					sdl->player->dirY * sdl->player->move_speed +
						(sdl->player->dirY > 0 ? 0.3 : -0.3) > 0)
			if (sdl->map->map[(int)(sdl->player->posY +
				sdl->player->dirY * sdl->player->move_speed +
					(sdl->player->dirY >
						0 ? 0.3 : -0.3))][(int)(sdl->player->posX)] < 1)
				sdl->player->posY += sdl->player->dirY *
					sdl->player->move_speed;
		if (sdl->player->posX + sdl->player->dirX *
			sdl->player->move_speed + (sdl->player->dirX > 0 ? 0.3 : -0.3) <
				sdl->map->cols && sdl->player->posX + sdl->player->dirX *
					sdl->player->move_speed + (sdl->player->dirX >
						0 ? 0.3 : -0.3) > 0)
			if (sdl->map->map[(int)(sdl->player->posY)][(int)(sdl->player->posX
				+ sdl->player->dirX * sdl->player->move_speed +
					(sdl->player->dirX > 0 ? 0.3 : -0.3))] < 1)
				sdl->player->posX += sdl->player->dirX *
					sdl->player->move_speed;
	}
	if (sdl->input.down)
	{
		if (sdl->player->posY - sdl->player->dirY *
			sdl->player->move_speed - (sdl->player->dirY > 0 ? 0.3 : -0.3) <
				sdl->map->rows && sdl->player->posY -
					sdl->player->dirY * sdl->player->move_speed -
						(sdl->player->dirY > 0 ? 0.3 : -0.3) > 0)
			if (sdl->map->map[(int)(sdl->player->posY -
				sdl->player->dirY * sdl->player->move_speed -
					(sdl->player->dirY >
						0 ? 0.3 : -0.3))][(int)(sdl->player->posX)] < 1)
				sdl->player->posY -= sdl->player->dirY *
					sdl->player->move_speed;
		if (sdl->player->posX - sdl->player->dirX *
			sdl->player->move_speed - (sdl->player->dirX > 0 ? 0.3 : -0.3) <
				sdl->map->cols && sdl->player->posX -
					sdl->player->dirX * sdl->player->move_speed -
						(sdl->player->dirX > 0 ? 0.3 : -0.3) > 0)
			if (sdl->map->map[(int)(sdl->player->posY)][(int)(sdl->player->posX
				- sdl->player->dirX * sdl->player->move_speed -
					(sdl->player->dirX > 0 ? 0.3 : -0.3))] < 1)
				sdl->player->posX -= sdl->player->dirX *
					sdl->player->move_speed;
	}
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

int		main(int argc, char **argv)
{
	t_sdl	*sdl;

	sdl = init();
	handle_arguments(sdl, argc, argv);
	sdl->player = init_player(sdl->map);
	print_map(sdl->map);
	open_textures(sdl);
	while (1)
	{
		if (!sdl->loading_done)
		{
			draw_loading_screen(sdl);
			sdl->loading_done = 1;
			SDL_RaiseWindow(sdl->window);
		}
		else
		{
			update_player_speed(sdl);
			draw_background(sdl);
			draw_map(sdl);
			draw_minimap(sdl);
			draw_fps(sdl);
		}
		SDL_UpdateWindowSurface(sdl->window);
		if (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
			else if (sdl->e.type == SDL_KEYDOWN)
				handle_keys_down(sdl);
			else if (sdl->e.type == SDL_KEYUP)
				handle_keys_up(sdl);
		}
		handle_player_movement(sdl);
	}

	close_sdl(sdl);
}
