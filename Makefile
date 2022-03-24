NAME = stl_test

#SRCS	= iterators/IOIterator.hpp iterators/ForwardIterator.hpp
SRCS	+= main.cpp
OBJS = $(SRCS:.hpp=.o)

CFLAGS = -Werror -Wextra -Wall
CC	= clang++

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) main.cpp -o $(NAME)

test: $(NAME)
	./test

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:		fclean all

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)

.PHONY: all clean fclean re test
