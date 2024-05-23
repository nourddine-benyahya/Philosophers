SRC = sources/parsing.c sources/methods.c sources/helper.c sources/philosophers.c sources/ft_itoa.c sources/actions.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I includes -pthread

NAME = philo_bonus

all : $(NAME)


$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c includes/philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -fr $(OBJ)

fclean : clean
	rm -fr $(NAME)

re : fclean all

.PHONY : all clean fclean re