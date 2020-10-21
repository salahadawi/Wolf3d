/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 19:08:45 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	handle_player_movement(t_sdl *sdl)
{
	if (sdl->input.left || sdl->input.right)
		handle_player_turning(sdl);
	if (sdl->input.up || sdl->input.down)
		handle_player_walking(sdl);
	if (sdl->input.jump)
		player_jump(sdl->player, &sdl->input.jump);
	if (sdl->input.crouch)
		player_crouch(sdl->player, 1);
	else if (sdl->player->crouching)
		player_crouch(sdl->player, 0);
}

int		main(int argc, char **argv)
{
	t_sdl	*sdl;

	sdl = init();
	handle_arguments(sdl, argc, argv);
	sdl->player = init_player(sdl->map);
	print_map(sdl->map);
	open_textures(sdl);
	while (1)
	{
		if (!sdl->loading_done)
		{
			draw_loading_screen(sdl);
			sdl->loading_done = 1;
			SDL_RaiseWindow(sdl->window);
		}
		else
			handle_frame(sdl);
		SDL_UpdateWindowSurface(sdl->window);
		handle_keys(sdl);
		handle_player_movement(sdl);
	}
	close_sdl(sdl);
}
