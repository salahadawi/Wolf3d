#include "wolf3d.h"

void	handle_error_sdl(char *message)
{
	ft_printf("SDL Error: %s SDL_ERROR %s\n", message, SDL_GetError());
	system("leaks wolf3d");
	exit(0);
}

void	handle_error(char *message)
{
	ft_printf("Error: %s\n", message);
	system("leaks wolf3d");
	exit(0);
}

void	close_sdl(t_sdl *sdl)
{
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	SDL_DestroyWindow(sdl->window);
	IMG_Quit();
	SDL_Quit();
	system("killall afplay");
	system("leaks wolf3d");
	exit(0);
}
