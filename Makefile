SRCS	= tests/miscellaneous.cpp
HEADERS = algorithm.hpp iterator.hpp type_traits.hpp utility.hpp
EXECS = $(SRCS:.cpp=)

CFLAGS = -Werror -Wextra -Wall --std=c++98
CC	= clang++

all:		$(EXECS)

$(EXECS): %: %.cpp $(HEADERS)
	$(CC) -o $@ $< $(CFLAGS)
	$@

clean:
	rm -f $(EXECS)

fclean: clean

re:		fclean all

.cpp:
	$(CC) $(CFLAGS) $< -o $(<:.cpp=)

.PHONY: all clean fclean re
