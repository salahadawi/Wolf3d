/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 13:47:23 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/19 00:00:48 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
	if (map->rows <= 1)
		handle_error("Map is empty or too small");
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
