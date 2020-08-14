/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/14 18:48:23 by sadawi           ###   ########.fr       */
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

			if (map->map[row][col] == PLAYER_SPAWN_POINT)
			{
				player->spawn_x = (double)col;
				player->spawn_y = (double)row;
				return;
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
	player->posX = player->spawn_x;
	player->posY = player->spawn_y;
	ft_printf("%f %f\n", player->spawn_x, player->spawn_y);
	player->dirX = -1;
	player->dirY = 0;
	player->planeX = 0;
	player->planeY = 0.66;
	player->move_speed = 0.1;
	player->rotation_speed = 0.05;
	return (player);
}

void put_pixel(SDL_Surface *screen, int x, int y, int color) {
	int *pixel;

	pixel = screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
	*pixel = color;
}

void	draw_vertical_line(t_sdl *sdl, int x, int y[2], int color)
{
	while (y[0] < y[1])
		put_pixel(sdl->screen, x, y[0]++, color);
}

void	draw_map(t_sdl *sdl)
{
	for(int x = 0; x < SCREEN_WIDTH; x++)
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
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if(rayDirX < 0)
      {
        stepX = -STEP_DIST_X;
        sideDistX = (sdl->player->posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = STEP_DIST_X;
        sideDistX = (mapX + 1.0 - sdl->player->posX) * deltaDistX;
      }
      if(rayDirY < 0)
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
        if(sideDistX < sideDistY)
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
        //Check if ray has hit a wall
        if(sdl->map->map[mapX][mapY] > 0) hit = 1;
      }
      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      if(side == 0) perpWallDist = (mapX - sdl->player->posX + (1 - stepX) / 2) / rayDirX;
      else          perpWallDist = (mapY - sdl->player->posY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
      if(drawEnd >= SCREEN_HEIGHT)drawEnd = SCREEN_HEIGHT - 1;

      //choose wall color
    int color;

	color = 0xFFFF;
      switch(sdl->map->map[mapX][mapY])
      {
        case 1:  color = 0xFF0000;    break; //red
        case 2:  color = 0x00FF00;  break; //green
        case 3:  color = 0x0000FF;   break; //blue
        case 4:  color = 0xFFFFFF;  break; //white
        default: color = 0x00FFFF; break; //yellow
      }

      //give x and y sides different brightness
	double wallX; //where exactly the wall was hit
	if (side == 0)
	{
		if (mapX - sdl->player->posX > 0)
		{
			color = sdl->tex[(int)(sdl->player->posX) % (TEX_WIDTH / 4)];
		} else {
			wallX = sdl->player->posY + perpWallDist * rayDirY;
		}
	}
	if (side == 1)
	{
		if (mapY - sdl->player->posY > 0)
			color = color / 5;
		color = color / 2;
		wallX = sdl->player->posX + perpWallDist * rayDirX;
	}

	//x coordinate on the texture
	int texX = (int)(wallX * (double)TEX_WIDTH);
	
	if(side == 0 && rayDirX > 0) texX = TEX_WIDTH - texX - 1;
	if(side == 1 && rayDirY < 0) texX = TEX_WIDTH - texX - 1;
	
      //draw the pixels of the stripe as a vertical line
      //verLine(x, drawStart, drawEnd, color);
	  color = sdl->tex[texX];
	  draw_vertical_line(sdl, x, (int[2]){drawStart, drawEnd}, color);
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
	color = 0xDDDDDD;
	while (i < pixel_amount / 2)
	{
		put_pixel(sdl->screen, i % SCREEN_WIDTH, i / SCREEN_WIDTH,
		(i / 100 - 25) % 9 ? color : color / 2);
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
		text = ft_sprintf("FPS: %.0f", CLOCKS_PER_SEC / (sdl->time_now - sdl->time_prev));
		sdl->text_surface = TTF_RenderText_Shaded(sdl->font, text,
		(SDL_Color){255, 255, 255, 0}, (SDL_Color){0, 0, 0, 0});
	}
	SDL_BlitSurface(sdl->text_surface, NULL, sdl->screen, NULL);
	sdl->time_prev = sdl->time_now;
}

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

void	draw_minimap(t_sdl *sdl)
{
	//unfinished
}

void	update_player_speed(t_sdl *sdl)
{
	sdl->time_prev = sdl->time_now;
	sdl->time_now = clock();
	sdl->player->move_speed = (sdl->time_now - sdl->time_prev) / 1000 / 150;
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
	SDL_BlitSurface(sdl->text_surface, NULL, sdl->screen, &(SDL_Rect){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50});
	sdl->time_prev = sdl->time_now;
}

void	create_textures(t_sdl *sdl)
{
	unsigned int	*tex;

	tex = (unsigned int *)malloc(sizeof(unsigned int) * TEX_HEIGHT * TEX_WIDTH);
	for(int x = 0; x < TEX_WIDTH; x++)
	for(int y = 0; y < TEX_HEIGHT; y++)
	{
		int xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
		tex[TEX_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor;
	}
	sdl->tex = tex;
}

int		main(int argc, char **argv)
{
	t_sdl *sdl;
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;
	int	loading = 1;

	sdl = init();
	handle_arguments(sdl, argc, argv);
	sdl->player = init_player(sdl->map);
	print_map(sdl->map);
	create_textures(sdl);
	while (1)
	{
		if (loading)
		{
			draw_loading_screen(sdl);
			loading = 0;
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
			{
				if (sdl->e.key.keysym.sym == SDLK_ESCAPE)
					break ;
				if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
					right = 1;
				if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
					left = 1;
				if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
					up = 1;
				if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
					down = 1;
			}
			else if (sdl->e.type == SDL_KEYUP)
			{
				if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
					right = 0;
				if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
					left = 0;
				if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
					up = 0;
				if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
					down = 0;
			}
		}
		if (right)
		{
			double oldDirX = sdl->player->dirX;
			sdl->player->dirX = sdl->player->dirX * cos(-sdl->player->rotation_speed) - sdl->player->dirY * sin(-sdl->player->rotation_speed);
			sdl->player->dirY = oldDirX * sin(-sdl->player->rotation_speed) + sdl->player->dirY * cos(-sdl->player->rotation_speed);
			double oldPlaneX = sdl->player->planeX;
			sdl->player->planeX = sdl->player->planeX * cos(-sdl->player->rotation_speed) - sdl->player->planeY * sin(-sdl->player->rotation_speed);
			sdl->player->planeY = oldPlaneX * sin(-sdl->player->rotation_speed) + sdl->player->planeY * cos(-sdl->player->rotation_speed);
		}
		if (left)
		{
			double oldDirX = sdl->player->dirX;
			sdl->player->dirX = sdl->player->dirX * cos(sdl->player->rotation_speed) - sdl->player->dirY * sin(sdl->player->rotation_speed);
			sdl->player->dirY = oldDirX * sin(sdl->player->rotation_speed) + sdl->player->dirY * cos(sdl->player->rotation_speed);
			double oldPlaneX = sdl->player->planeX;
			sdl->player->planeX = sdl->player->planeX * cos(sdl->player->rotation_speed) - sdl->player->planeY * sin(sdl->player->rotation_speed);
			sdl->player->planeY = oldPlaneX * sin(sdl->player->rotation_speed) + sdl->player->planeY * cos(sdl->player->rotation_speed);
		}
		if (up)
		{
			if (sdl->map->map[(int)(sdl->player->posX + sdl->player->dirX * sdl->player->move_speed)][(int)(sdl->player->posY)] < 1)
				sdl->player->posX += sdl->player->dirX * sdl->player->move_speed;
			if (sdl->map->map[(int)(sdl->player->posX)][(int)(sdl->player->posY + sdl->player->dirY * sdl->player->move_speed)] < 1)
				sdl->player->posY += sdl->player->dirY * sdl->player->move_speed;
		}
		if (down)
		{
			if (sdl->map->map[(int)(sdl->player->posX - sdl->player->dirX * sdl->player->move_speed)][(int)(sdl->player->posY)] < 1)
				sdl->player->posX -= sdl->player->dirX * sdl->player->move_speed;
			if (sdl->map->map[(int)(sdl->player->posX)][(int)(sdl->player->posY - sdl->player->dirY * sdl->player->move_speed)] < 1)
				sdl->player->posY -= sdl->player->dirY * sdl->player->move_speed;
		}
		//clear_surface(sdl->screen);
	}
	close_sdl(sdl);
}
