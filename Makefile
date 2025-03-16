NAME		= pipex

SRCS		= pipex.c
OBJS		= $(SRCS:.c=.o)
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

CC			= cc
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror -I. -fsanitize=address

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
		make -C $(LIBFT_DIR)

%.o:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM)	$(OBJS)
	make clean -C $(LIBFT_DIR)

fclean:	clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re:	fclean all

.PHONY: all clean fclean re
