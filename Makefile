SRC = sources/parsing.c sources/cleaning.c sources/philosophers.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I includes -pthread -fsanitize=address -g

NAME = philosophers

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