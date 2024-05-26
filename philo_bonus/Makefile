SRC = sources/parsing_bonus.c sources/methods_bonus.c sources/helper_bonus.c \
		sources/philosophers_bonus.c sources/ft_itoa_bonus.c sources/actions_bonus.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I includes -pthread

NAME = philo_bonus

all : $(NAME)


$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c includes/philosophers_bonus.h includes/env_bonus.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -fr $(OBJ)

fclean : clean
	rm -fr $(NAME)

re : fclean all

.PHONY : all clean fclean re