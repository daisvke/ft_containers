# E X E C U T A B L E  N A M E #

NAME			=	ft_containers


# C X X  F L A G S #
# c++ compiler is mentioned on the subject, 
# but clang++ shows more errors
CXX				=	clang++
CXXFLAGS		=	-Wall -Wextra -std=c++98 -Werror
DEBUG			=	-g3
DCXX			=	clang++


# S O U R C E  F I L E S #
SRC				=	main.cpp
# O B J .  F I L E S #
OBJ				=	main.o

# I N C .  F I L E S #

INCS			=	algorithm.hpp \
					vector.hpp \
					type_traits.hpp \
					iterator.hpp \
					utility.hpp \
					map.hpp \
					tree.hpp \
					stack.hpp \
					set.hpp


#  B U I L D  R U L E S #

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
		$(CXX) -o $@ $(OBJ)

$(OBJ): $(SRC) $(INCS)
		$(CXX) $(CXXFLAGS) -c $< -o $@


# C L E A N  &  O T H E R  R U L E S #

debug: $(OBJ) $(INCS)
		$(CXX) $(DEBUG) -o $@ $(OBJ)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME) debug

re: fclean all
