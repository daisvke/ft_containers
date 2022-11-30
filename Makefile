# E X E C U T A B L E  N A M E #

NAME			=	ft_containers


# C X X  F L A G S #

CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -std=c++98 #-Werror
DEBUG			=	-g3


# S O U R C E  F I L E S #

SRCS			=	main.cpp


# O B J .  F I L E S #

OBJS_DIR		=	objs/
OBJS_FILES		=	$(SRCS:.cpp=.o)
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_FILES))


# I N C .  F I L E S #

INC_DIR			=	incs/
INC_FILES		=	algorithm.hpp \
					vector.hpp \
					type_traits.hpp \
					iterator.hpp \
					utility.hpp \
					map.hpp
INCS			=	$(addprefix $(INC_DIR), $(INC_FILES))


#  B U I L D  R U L E S #

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) -o $@ $(OBJS)

$(OBJS): $(SRCS) $(INCS)
		mkdir -p objs/
		$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(DEBUG) -c $< -o $@


# C L E A N  &  O T H E R  R U L E S #

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
