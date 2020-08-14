/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 13:47:23 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/14 13:48:06 by sadawi           ###   ########.fr       */
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
