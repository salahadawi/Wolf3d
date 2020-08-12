/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 19:47:21 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/12 20:43:07 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	close_sdl(t_sdl *sdl)
{
	// for (t_texture *tmp = sdl->textures->next; sdl->textures; tmp = sdl->textures->next)
	// {
	// 	free(sdl->textures);
	// 	sdl->textures = tmp;
	// }
	SDL_DestroyRenderer(sdl->renderer);
	sdl->renderer = NULL;
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void	handle_error_sdl(char *message)
{
	ft_printf("Visualizer: %s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void	handle_error(char *message)
{
	ft_printf("Visualizer: %s\n", message);
	exit(0);
}

void	init(t_sdl *sdl)
{
	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		handle_error("Malloc failed");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error_sdl("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("Lem-in", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error_sdl("Window could not be created!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		handle_error_sdl("Renderer could not be created!");
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error_sdl("SDL image could not initialize!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
}

int	main(void)
{
	t_sdl *sdl;

	sdl = NULL;
	init(sdl);
	while (1)
	{
		if (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
			// else if (sdl->e.type == SDL_KEYDOWN)
			// {

			// }
		}
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
