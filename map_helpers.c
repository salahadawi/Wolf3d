/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:06:19 by alcohen           #+#    #+#             */
/*   Updated: 2020/09/01 16:15:28 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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

/*
** Checks that the map only contains spaces and
** integers up to different wall types for textures
*/

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
