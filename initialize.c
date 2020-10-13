#include "wolf3d.h"

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
