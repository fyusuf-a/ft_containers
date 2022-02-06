NAME = libstl.a

#SRCS	= iterators/IOIterator.hpp iterators/ForwardIterator.hpp
SRCS	+= Vector.hpp
OBJS = $(SRCS:.hpp=.o)

CFLAGS = -Werror -Wextra -Wall
CC	= clang++

all:		$(NAME)

test:		$(NAME)
	$(CC) $(CFLAGS) main.cpp -L. -lstl

$(NAME):	$(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:		fclean all

.hpp.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.hpp=.o)

.PHONY: all clean fclean re debug test
.SUFFIXES:
.SUFFIXES: .hpp .o
