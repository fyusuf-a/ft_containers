SRCS	= tests/miscellaneous.cpp tests/vector.cpp tests/map.cpp tests/tree.cpp \
		  tests/stack.cpp
HEADERS = algorithm.hpp iterator.hpp type_traits.hpp utility.hpp
EXECS = $(SRCS:.cpp=)
EXECS_STD = $(SRCS:.cpp=_std)

CFLAGS = -Werror -Wextra -Wall --std=c++98
CC	= clang++

all:		$(EXECS) $(EXECS_STD)

$(EXECS): %: %.cpp $(HEADERS)
	$(CC) -o $@ $< $(CFLAGS)
	$@

$(EXECS_STD): %_std: %.cpp $(HEADERS)
	$(CC) -D STD -o $@ $< $(CFLAGS)
	$@

clean:
	rm -f $(EXECS) $(EXECS_STD)

fclean: clean

re:		fclean all

.PHONY: all clean fclean re
