/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/20 19:59:25 by sadawi           ###   ########.fr       */
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
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <time.h>

#include <stdio.h> //

#define STEP_DIST_X		1
#define STEP_DIST_Y		1
#define LIGHTING_INTENSITY 1
#define OUT_OF_BOUNDS_COLOR 0x550000

/*
** Player
*/

#define CROUCH_AMT 100
#define CROUCH_MOVE_SPEED_MULT 0.3
#define MOVE_SPEED 0.1
#define ROTATION_SPEED 0.05;
#define SPAWN_POINT -1


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
	int				cam_height;
	int				crouching;
	int				busy;
}					t_player;

typedef struct		s_input
{
	int				up;
	int				down;
	int				right;
	int				left;
	int				jump;
	int				crouch;
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
	double			wall_dist;
}					t_sdl;

void	handle_arguments(t_sdl *sdl, int argc, char **argv);

void	handle_error(char *message);

void	print_map(t_map *map);

void	player_jump(t_player *player, int *input_jump);

void	player_crouch(t_player *player, int crouch);

void	modify_pixel_multiply(SDL_Surface *screen, int x, int y, double amount);

void	modify_pixel_remove(SDL_Surface *screen, int x, int y, int color);

void	modify_pixel_add(SDL_Surface *screen, int x, int y, int color);

void	add_fog_to_pixel(SDL_Surface *screen, int x, int y, double wall_dist);

#endif
