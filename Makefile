NAME = hashgraph

SRC = Event.cpp Person.cpp main.cpp md5.cpp

FRAMEWORKS = -framework OpenGl

BREW_INC = -I ~/.brew/include

SDL_LINK = -g  -L ~/.brew/lib -l SDL2

FLAGS = -Wfatal-errors

all: $(NAME)

$(NAME): $(SRC)
	g++ -g -std=c++11 $(FLAGS) $(SRC) -o $(NAME) $(FRAMEWORKS) $(BREW_INC) $(SDL_LINK)

clean:
	rm -f *.o
	rm -f Log*
	rm -rf *.dSYM

fclean: clean
	rm -f $(NAME)

re: fclean all
