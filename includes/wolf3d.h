/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 20:10:38 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# define SCREEN_WIDTH 1600
# define SCREEN_HEIGHT 900

# include "../libft/includes/libft.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

typedef struct			s_sdl
{
	SDL_Window			*window;
	SDL_Surface			*screen;
	SDL_Surface			*image;
	SDL_Event			e;
	int					img_flags;
	SDL_Renderer		*renderer;
	//t_texture			*textures;
}						t_sdl;

#endif