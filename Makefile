NAME = ircserv

BUILD_FLAGS = -Wall -Wextra -std=c++17 -Werror

SRC = src/main src/Server src/errors src/User src/Channel src/ServerHelp

H	= includes/irc includes/Server includes/User includes/Channel

SRC_SUFF = $(addsuffix .cpp, $(SRC))
HEADER_SUFF = $(addsuffix .hpp, $(H))

SRCSO = $(addsuffix .o, $(SRC))

.PHONY: all clean fclean re
all: $(NAME)

$(NAME): $(SRC_SUFF) $(HEADER_SUFF)
	c++ $(BUILD_FLAGS) $(SRC_SUFF) -o $(NAME)

clean:
	rm -f $(SRCSO)

fclean: clean
	rm -f $(NAME)

re: fclean all