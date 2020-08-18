/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/18 18:35:28 by sadawi           ###   ########.fr       */
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
#define TEX_WIDTH 512
#define TEX_HEIGHT 512

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
	int				jump_height;
}					t_player;

typedef struct		s_input
{
	int				up;
	int				down;
	int				right;
	int				left;
	int				jump;
}					t_input;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	int				p_spawn_point;
	t_map			*map;
	t_player		*player;
	TTF_Font		*font;
	SDL_Surface		*text_surface;
	double			time_prev;
	double			time_now;
	unsigned int	*tex;
	t_input			input;
	int				pixelation;
	SDL_Surface		*texture;
}					t_sdl;

void	handle_arguments(t_sdl *sdl, int argc, char **argv);

void	handle_error(char *message);

void	print_map(t_map *map);

void	handle_jump_height(t_sdl *sdl);

#endif
