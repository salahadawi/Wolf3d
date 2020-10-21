/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:56:17 by sadawi            #+#    #+#             */
/*   Updated: 2020/10/13 18:56:18 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	handle_error_sdl(char *message)
{
	ft_printf("SDL Error: %s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void	handle_error(char *message)
{
	ft_printf("Error: %s\n", message);
	exit(0);
}

void	close_sdl(t_sdl *sdl)
{
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	SDL_DestroyWindow(sdl->window);
	IMG_Quit();
	SDL_Quit();
	exit(0);
}
