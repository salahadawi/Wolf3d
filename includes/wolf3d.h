/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/14 18:17:50 by alcohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# define SCREEN_WIDTH	1024
# define SCREEN_HEIGHT	680
# define PROGRAM_TITLE	"Wolf3D"

# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <math.h>
# include "SDL.h"
# include "SDL_image.h"
# include "SDL_ttf.h"
# include <time.h>

#define STEP_DIST_X		1
#define STEP_DIST_Y		1
#define PLAYER_SPAWN_POINT -1
#define TEX_WIDTH 64
#define TEX_HEIGHT 64

typedef	struct			s_texture
{
	SDL_Texture			*texture;
	int					width;
	int					height;
	struct s_texture	*next;
}						t_texture;

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
	double			spawn_x;
	double			spawn_y;
	double			move_speed;
	double			rotation_speed;
}					t_player;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	int				p_spawn_point;
	t_texture		*textures;
	t_map			*map;
	t_player		*player;
	TTF_Font		*font;
	SDL_Surface		*text_surface;
	double			time_prev;
	double			time_now;
	unsigned int	*tex;
}					t_sdl;

void	handle_arguments(t_sdl *sdl, int argc, char **argv);

void	handle_error(char *message);

void	print_map(t_map *map);

#endif
