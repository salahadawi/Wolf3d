/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/13 12:51:34 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	close_sdl(t_sdl *sdl)
{
	SDL_DestroyRenderer(sdl->renderer);
	sdl->renderer = NULL;
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void	handle_error_sdl(char *message)
{
	ft_printf("Visualizer: %s SDL_ERROR %s\n", message, SDL_GetError());
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
	sdl->window = SDL_CreateWindow("Lem-in", 100, 100,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error_sdl("Window could not be created!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		handle_error_sdl("Renderer could not be created!");
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error_sdl("SDL image could not initialize!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	return (sdl);
}

void	malloc_extra_row(t_map *s_map)
{
	int **tmp;
	int i;

	if (!(tmp = (int**)ft_memalloc(sizeof(int*) * (s_map->rows))))
		handle_error("Malloc failed");
	ft_memcpy(tmp, s_map->map, sizeof(int**) * s_map->rows);
	if (s_map->rows)
		free(s_map->map);
	if (!(s_map->map = (int**)ft_memalloc(sizeof(int*) * (s_map->rows + 1))))
		handle_error("Malloc failed");
	i = 0;
	while (i < s_map->rows)
	{
		s_map->map[i] = tmp[i];
		i++;
	}
	free(tmp);
}

void	check_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]) && line[i] != ' ' && !ft_isdigit_neg(&line[i]))
			handle_error("Map contains non-digit symbol");
		i++;
	}
}

int		bigger_than_int(char *line)
{
	int i;

	i = 0;
	while (ft_isdigit(line[i]))
		i++;
	if (i == 10)
	{
		if (ft_atoilong(&line[0]) > 2147483647)
			return (1);
	}
	return (!(i < 11));
}

void	check_count(int count, t_map *s_map)
{
	if (!count)
		handle_error("Invalid row in file");
	if (s_map->cols)
		if (count != s_map->cols)
			handle_error("Rows differ in length");
}

int		count_ints(char *line, t_map *s_map)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] == ' ' && line[i])
			i++;
		if (ft_isdigit(line[i]) || ft_isdigit_neg(&line[i]))
		{
			if (line[i] == '-')
				i++;
			if (bigger_than_int(&line[i]))
				handle_error("Map contains number bigger than MAX_INT");
			while (ft_isdigit(line[i]))
				i++;
			count++;
		}
	}
	check_count(count, s_map);
	return (count);
}

void	store_map_line(char *line, t_map *s_map)
{
	int i;
	int col;
	int count;

	i = 0;
	col = 0;
	check_line(line);
	count = count_ints(line, s_map);
	if (!(s_map->map[s_map->rows] = (int*)ft_memalloc(sizeof(int) * count)))
		handle_error("Malloc failed");
	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if (line[i])
			s_map->map[s_map->rows][col++] = ft_atoi(&line[i]);
		while (line[i] != ' ' && line[i])
			i++;
	}
	s_map->rows++;
	s_map->cols = col;
}

t_map	*store_map(char *filename)
{
	int		fd;
	char	*line;
	t_map	*map;

	if (!(map = (t_map*)ft_memalloc(sizeof(*map))))
		handle_error("Malloc failed");
	map->name = ft_strdup(filename);
	if ((fd = open(filename, O_RDONLY)) == -1)
		handle_error("File could not be opened!");
	while (get_next_line(fd, &line) > 0)
	{
		malloc_extra_row(map);
		store_map_line(line, map);
		free(line);
	}
	if (map->rows == 0)
		handle_error("Map is empty");
	close(fd);
	return (map);
}

void	print_usage_and_exit(void)
{
	ft_printf("Usage: ./wolf3d [file]\n");
	exit(0);
}

void	handle_arguments(t_sdl *sdl, int argc, char **argv)
{
	if (argc < 2)
		print_usage_and_exit();
	if (argc > 2)
		handle_error("Too many arguments!");
	sdl->map = store_map(argv[1]);
}

void	print_map(t_map *s_map)
{
	int i;
	int j;

	i = 0;
	while (i < s_map->rows)
	{
		j = 0;
		while (j < s_map->cols)
			ft_printf("%3d ", s_map->map[i][j++]);
		ft_printf("\n");
		i++;
	}
}

int		main(int argc, char **argv)
{
	t_sdl *sdl;

	sdl = init();
	handle_arguments(sdl, argc, argv);
	SDL_SetRenderDrawColor(sdl->renderer, 0x77, 0x77, 0x77, 0xFF);
	SDL_RenderClear(sdl->renderer);
	print_map(sdl->map);
	while (1)
	{
		if (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
			else if (sdl->e.type == SDL_KEYDOWN)
			{
				if (sdl->e.key.keysym.sym == SDLK_ESCAPE)
					break ;
			}
		}
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
