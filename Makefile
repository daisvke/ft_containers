NAME			=	ft_containers
CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -std=c++98 #-Werror
DEBUG			=	-g3

SRCS			=	main.cpp
OBJS_DIR		=	objs/
OBJS_FILES		=	$(SRCS:.cpp=.o)
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_FILES))
INCS			=	algorithm.hpp \
					vector.hpp \
					type_traits.hpp \
					iterator.hpp \
					utility.hpp

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) -o $@ $(OBJS)

$(OBJS): $(SRCS)
		mkdir -p objs/
		$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
