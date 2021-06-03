# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldalle-a <ldalle-a@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/30 13:17:01 by ldalle-a          #+#    #+#              #
#    Updated: 2021/04/30 13:17:05 by ldalle-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= cub3D
MINILIBX_PATH   = ./miniLibX
SRCS 			= ./cub3d.c \
					./get_next_line.c \
					./get_next_line_utils.c \
					./parsemap.c \
					./screenshot.c \
					
OBJS			= ${SRCS:.c=.o}
INCLUDE 		= cube3d.h
LIBFT 			= libft
MINILIBX 		= miniLibX
CC				= gcc -Wall -Wextra -Werror
RM				= rm -f
MLXFLAGS 		= -lmlx -framework OpenGl -framework Appkit

# -I Add the directory dir to the list of directories to be searched for header files
# -L Searches the library when linking

all:			${NAME}
$(NAME):		${OBJS} 
				@$(CC) $(MLXFLAGS) -I./ $(OBJS) -o $@ 
clean:			
				@${RM} ${OBJS}
fclean:		
				@${RM} ${NAME}
re:				fclean all

# make other makefiles compile with the -C flag
# The -C flag makes you go to the appropriate path and do the asked command

minilibx_all:
	make -C $(MINILIBX_PATH) all
	cp ./minilibX/libmlx.a libmlx.a

minilibx_clean:
	make -C $(MINILIBX_PATH) clean
	$(RM) libmlx.a
				
.PHONY: all fclean clean re
