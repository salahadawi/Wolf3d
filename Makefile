# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 19:26:38 by sadawi            #+#    #+#              #
#    Updated: 2020/10/13 19:09:14 by sadawi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
CFILES = main.c handle_map.c player_movement.c modify_pixels.c draw_line.c \
		map_helpers.c draw_view.c minimap.c error_exit_handling.c \
		player_turning.c raycasting.c pixel_operations.c initialize.c \
		handle_keys.c extra_helpers.c extra_helpers_2.c draw_view_lines.c \
		raycasting_2.c
SRCS=$(addprefix src/, $(CFILES))
INCLUDES_LINUX = `sdl2-config --cflags --libs` -I libft/includes -I includes
FLAGS = -Wall -Wextra -Werror -O3
RUN_LIB = make --no-print-directory -C libft/
LIB = libft/libft.a
LIBS_LINUX = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
INCLUDES = includes \
			-I SDL2.framework/Headers \
			-I SDL2_image.framework/Headers \
			-I SDL2_ttf.framework/Headers
FRAMEWORKS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf

all:
	@$(RUN_LIB)
	@make --no-print-director $(NAME)

$(NAME): $(SRCS) libft/
	@echo Compiling $(NAME)...
	gcc $(FLAGS) $(FRAMEWORKS) -F ./ $(SRCS) $(LIB) -I $(INCLUDES) -o $(NAME) -rpath @executable_path
	@echo $(NAME) compiled succesfully!

lib:
	@$(RUN_LIB)

noflags:
	make -C libft/
	gcc $(FLAGS) $(FRAMEWORKS) -F ./ $(SRCS) $(LIB) -I $(INCLUDES) -o $(NAME) -rpath @executable_path

linux:
	@rm -rf objs
	@echo Compiling $(NAME) without flags...
	@gcc $(INCLUDES_LINUX) -c $(SRCS)
	@mkdir objs
	@mv $(notdir $(SRCS:.c=.o)) objs
	@gcc $(INCLUDES_LINUX) -o $(NAME) $(OBJS) libft/libft.a $(LIBS_LINUX)
	@echo $(NAME) linux compiled without flags succesfully!

clean:
	@/bin/rm -f $(OBJS)
	@rm -rf objs
	@make -C libft/ clean
	@echo Clean successful!

clean_binary:
	@/bin/rm -f $(OBJS)
	@rm -rf objs
	@echo Clean successful!

fclean: clean_binary
	@/bin/rm -f $(NAME)
	@make -C libft/ fclean
	@echo Clean successful!

re: fclean all
