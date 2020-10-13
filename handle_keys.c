/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:56:05 by alcohen           #+#    #+#             */
/*   Updated: 2020/10/13 18:58:06 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	handle_keys_down(t_sdl *sdl)
{
	if (sdl->e.key.keysym.sym == SDLK_ESCAPE)
		close_sdl(sdl);
	if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
		sdl->input.left = 1;
	if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
		sdl->input.right = 1;
	if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
		sdl->input.up = 1;
	if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
		sdl->input.down = 1;
	if (sdl->e.key.keysym.sym == SDLK_SPACE)
		sdl->input.jump = 1;
	if (sdl->e.key.keysym.sym == SDLK_e)
		sdl->pixelation++;
	if (sdl->e.key.keysym.sym == SDLK_q)
		sdl->pixelation ? sdl->pixelation-- : (void)sdl->pixelation;
	if (sdl->e.key.keysym.sym == SDLK_LCTRL)
		sdl->input.crouch = 1;
	if (sdl->e.key.keysym.sym == SDLK_x && sdl->player->vertical_fov <= 4.0)
		sdl->player->vertical_fov += FOV_CHANGE_AMT;
	if (sdl->e.key.keysym.sym == SDLK_c && sdl->player->vertical_fov >= 1.0)
		sdl->player->vertical_fov -= FOV_CHANGE_AMT;
}

void	handle_keys_up(t_sdl *sdl)
{
	if (sdl->e.key.keysym.sym == SDLK_RIGHT || sdl->e.key.keysym.sym == SDLK_d)
		sdl->input.left = 0;
	if (sdl->e.key.keysym.sym == SDLK_LEFT || sdl->e.key.keysym.sym == SDLK_a)
		sdl->input.right = 0;
	if (sdl->e.key.keysym.sym == SDLK_UP || sdl->e.key.keysym.sym == SDLK_w)
		sdl->input.up = 0;
	if (sdl->e.key.keysym.sym == SDLK_DOWN || sdl->e.key.keysym.sym == SDLK_s)
		sdl->input.down = 0;
	if (sdl->e.key.keysym.sym == SDLK_LCTRL)
		sdl->input.crouch = 0;
}

void	handle_keys(t_sdl *sdl)
{
	if (SDL_PollEvent(&sdl->e))
	{
		if (sdl->e.type == SDL_QUIT)
			close_sdl(sdl);
		else if (sdl->e.type == SDL_KEYDOWN)
			handle_keys_down(sdl);
		else if (sdl->e.type == SDL_KEYUP)
			handle_keys_up(sdl);
	}
}
