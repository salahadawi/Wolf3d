/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:09:05 by sadawi           ###   ########.fr       */
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

/*
** Textures/visuals
** TEXTURES has 4 textures for each block.
** The textures are determined by the block's number on the map.
** Each block must have 4 textures defined.
** For example a map block '5' means TEXTURES must have 20 textures
*/

# define TEXTURES "wall.png stone.png alex.png white.png"

# define TEXTURES_FOLDER "textures/"
# define LIGHTING_INTENSITY 1
# define OUT_OF_BOUNDS_COLOR 0x550000

/*
** Player
*/

# define CROUCH_AMT 100
# define CROUCH_MOVE_SPEED_MULT 0.3
# define MOVE_SPEED 0.1
# define ROTATION_SPEED 0.05
# define SPAWN_POINT -1
# define VERTICAL_FOV_DIV	2.0
# define FOV_CHANGE_AMT 0.05
# define STEP_DIST_X		1
# define STEP_DIST_Y		1

typedef	struct			s_texture
{
	SDL_Texture			*texture;
	int					width;
	int					height;
	struct s_texture	*next;
}						t_texture;

typedef struct			s_map
{
	char				*name;
	int					**map;
	int					rows;
	int					cols;
	int					max_num;
}						t_map;

typedef struct			s_player
{
	double				x;
	double				y;
	double				dir_x;
	double				dir_y;
	double				plane_x;
	double				plane_y;
	double				spawn_x;
	double				spawn_y;
	double				move_speed;
	double				rotation_speed;
	int					cam_height;
	int					crouching;
	int					busy;
	double				vertical_fov;
}						t_player;

typedef struct			s_input
{
	int					up;
	int					down;
	int					right;
	int					left;
	int					jump;
	int					crouch;
}						t_input;

typedef struct			s_math
{
	int		wall_side;
	int		texture_x;
	int		map_x;
	int		map_y;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perpendicular_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_idx;
}						t_math;

typedef struct			s_sdl
{
	SDL_Window			*window;
	SDL_Surface			*screen;
	SDL_Event			e;
	int					img_flags;
	SDL_Renderer		*renderer;
	int					p_spawn_point;
	t_map				*map;
	t_player			*player;
	TTF_Font			*font;
	SDL_Surface			*text_surface;
	double				time_prev;
	double				time_now;
	unsigned int		*tex;
	t_input				input;
	int					pixelation;
	SDL_Surface			**textures;
	double				wall_dist;
	int					loading_done;
	int					textures_amount;
	t_math				math;
}						t_sdl;

t_sdl					*init(void);
void					set_spawn_point(t_player *player, t_map *map);
t_player				*init_player(t_map *map);
void					handle_arguments(t_sdl *sdl, int argc, char **argv);
void					handle_error(char *message);
void					print_map(t_map *map);
void					player_jump(t_player *player, int *input_jump);
void					player_crouch(t_player *player, int crouch);
void					modify_pixel_multiply(SDL_Surface *screen,
												int x, int y, double amount);
void					modify_pixel_remove(SDL_Surface *screen,
												int x, int y, int color);
void					modify_pixel_add(SDL_Surface *screen,
												int x, int y, int color);
void					add_fog_to_pixel(SDL_Surface *screen,
												int x, int y, double wall_dist);
void					put_pixel(SDL_Surface *screen,
												int x, int y, int color);
void					draw_line(t_sdl *sdl, int x[2], int y[2], int color);
void					check_count(int count, t_map *s_map);
int						count_ints(char *line, t_map *s_map);
void					check_line(char *line);
int						bigger_than_int(char *line);
void					player_turn_right(t_player *player, double old_dir_x,
															double old_plane_x);
void					player_turn_left(t_player *player, double old_dir_x,
															double old_plane_x);
void					player_walk_forward(t_player *player, t_map *map);
void					player_walk_backward(t_player *player, t_map *map);
void					draw_vertical_line_from_image(t_sdl *sdl,
														SDL_Surface *texture,
														int x[2], int y[2]);
int						get_pixel(SDL_Surface *screen, int x, int y);
void					draw_x(t_sdl *sdl, int x);
void					draw_out_of_bounds_line(t_sdl *sdl, int x);
void					draw_map_line(t_sdl *sdl, int x);
void					draw_vertical_line(t_sdl *sdl, int x, int y[2],
											int color);
void					draw_box(SDL_Surface *screen, int xywh[4], int color,
									void (*f)(SDL_Surface*, int, int, int));
void					draw_minimap_map(t_sdl *sdl);
void					draw_minimap_fov_cone(t_sdl *sdl);
void					draw_minimap(t_sdl *sdl);
void					put_minimap_pixel(SDL_Surface *screen, int x, int y,
									int color);
void					close_sdl(t_sdl *sdl);
void					handle_error(char *message);
void					handle_error_sdl(char *message);
void					handle_player_turning(t_sdl *sdl);
void					player_turn_left(t_player *player, double old_dir_x,
							double old_plane_x);
void					player_turn_right(t_player *player, double old_dir_x,
							double old_plane_x);
void					handle_player_walking(t_sdl *sdl);
void					calculate_ray_len(t_sdl *sdl);
void					calculate_ray_pos(t_sdl *sdl, int x);
void					calculate_perpendicular_dist(t_sdl *sdl);
void					calculate_line_height(t_sdl *sdl);
void					calculate_texture_x(t_sdl *sdl);
void					handle_keys_down(t_sdl *sdl);
void					handle_keys_up(t_sdl *sdl);
void					handle_keys(t_sdl *sdl);
void					open_textures(t_sdl *sdl);
void					draw_loading_screen(t_sdl *sdl);
void					draw_fps(t_sdl *sdl);
void					handle_frame(t_sdl *sdl);
void					clear_surface(SDL_Surface *surface);
void					get_map_box(t_sdl *sdl);
void					update_player_speed(t_sdl *sdl);
void					draw_background(t_sdl *sdl);
void					draw_map(t_sdl *sdl);
void					draw_background(t_sdl *sdl);
void					draw_map(t_sdl *sdl);
void					find_nearest_wall(t_sdl *sdl);
void					calculate_side_dist(t_sdl *sdl);

#endif
