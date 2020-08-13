/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/13 16:20:40 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# define SCREEN_WIDTH 1024
# define SCREEN_HEIGHT 680

# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <math.h>
# include "SDL.h"
# include "SDL_image.h"

typedef struct		s_map
{
	char			*name;
	int				**map;
	int				rows;
	int				cols;
}					t_map;

typedef struct		s_player
{
	double			posX;
	double			posY;
	double			dirX;
	double			dirY;
	double			planeX;
	double			planeY;
}					t_player;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	//t_texture		*textures;
	t_map			*map;
	t_player		*player;
}					t_sdl;

#endif
