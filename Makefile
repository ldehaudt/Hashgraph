NAME = hashgraph

SRC = *.cpp

FRAMEWORKS = -framework OpenGl

BREW_INC = -I ~/.brew/include

SDL_LINK = -g  -L ~/.brew/lib -l SDL2

FLAGS = -Wfatal-errors

all: $(NAME)

$(NAME):
	g++ -g $(FLAGS) $(SRC) -o $(NAME) $(FRAMEWORKS) $(BREW_INC) $(SDL_LINK)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all
