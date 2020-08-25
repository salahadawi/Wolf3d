# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alcohen <alcohen@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/10 19:26:38 by sadawi            #+#    #+#              #
#    Updated: 2020/08/25 19:51:30 by alcohen          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
SRCS = main.c handle_map.c player_movement.c modify_pixels.c draw_line.c
OBJS = $(addprefix objs/, $(notdir $(SRCS:.c=.o)))
#INCLUDES = -I libft/includes -I includes \
-I ~/.brew/Cellar/sdl2/2.0.12_1/include/SDL2/ \
-I ~/.brew/Cellar/sdl2_image/2.0.5/include/SDL2 \
-I ~/.brew/Cellar/sdl2_ttf/2.0.15/include/SDL2
INCLUDES_LINUX = `sdl2-config --cflags --libs` -I libft/includes -I includes
FLAGS = -Wall -Wextra -Werror -O3
RUN_LIB = make --no-print-directory -C libft/
LIB = libft/libft.a
LIBS_LINUX = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
INCLUDE = includes

all:
	@$(RUN_LIB)
	@make --no-print-director $(NAME)

$(NAME): $(SRCS) libft/
	@rm -rf objs
	@echo Compiling $(NAME)...
	@gcc $(FLAGS) $(INCLUDES) -c $(SRCS)
	@mkdir objs
	@mv $(notdir $(SRCS:.c=.o)) objs
	@gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS) libft/libft.a
	@echo $(NAME) compiled succesfully!

lib:
	@$(RUN_LIB)

noflags:
	make -C libft/
	gcc $(FLAGS) -framework SDL2 -framework SDL2_image -F ./ $(SRCS) $(LIB) -I $(INCLUDE) -I SDL2.framework/Headers -I SDL2_image.framework/Headers -o $(NAME) -rpath @executable_path
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
