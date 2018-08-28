NAME = hashgraph

SRC = Event.cpp Person.cpp main.cpp md5.cpp Showcaser.cpp

OBJ = $(SRC:.cpp=.o)

FRAMEWORKS = -framework OpenGl

GREEN = \033[1;38;2;0;175;117m
RED = \033[1;38;2;225;20;20m
WHITE = \033[1;38;2;255;251;214m
YELLO = \033[1;38;2;255;200;0m

BREW_INC = -I ~/.brew/include

SDL_LINK = -g  -L ~/.brew/lib -l SDL2

CPPFLAGS = -Wall -Wextra -Werror -O2

CPP = @clang++ -std=c++11

all: $(NAME)

$(NAME): $(OBJ) brew compile gaetan

clean:
	@echo "${RED}Removing object files"	
	@rm -f *.o
	@rm -f Log*
	@rm -rf *.dSYM

fclean: clean
	@echo "${RED}Removing Executable >.<"	
	@rm -f $(NAME)

re: fclean all

brew:
ifneq ("$(shell test -e $(HOME)/.brew && echo ex)" , "ex")
	@echo "BREW INSTALLING ..."
	@mkdir $(HOME)/.brew && curl -fsSL https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C $(HOME)/.brew
	@mkdir -p /tmp/.$(whoami)-brew-locks
	@mkdir -p $(HOME)/.brew/var/homebrew
	@ln -s /tmp/.$(whoami)-brew-locks $(HOME)/.brew/var/homebrew/locks
	@export PATH="$(HOME)/.brew/bin:$(PATH)"
	@brew update && brew upgrade
	@echo "${YELLO}INSTALLING SDL2: please be patient"
endif

compile :
	@echo "${YELLO}Checking for graphics library ..."	
	@brew install sdl2
	@echo "${YELLO}Compiling ...${WHITE}"
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME) $(FRAMEWORKS) $(BREW_INC) $(SDL_LINK)

%.o: %.cpp %.hpp
	@echo "${WHITE}Creating $@"
	$(CPP) -c $< $(BREW_INC)

gaetan: 
	@echo "${RED}It's a Gaetan ???${WHITE}"
	@sleep 1
	@echo "NNNNNNNNNNNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNNNNNNNmh"
	@echo "NNNNNNNNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNNNNNmd"
	@echo "NNNNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdhyssssyhddmNMNNMMMMMMMMMMMMMMMMMMMNNNNNNNmd"
	@echo "NNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmso:------:://+oo/:oNMMMMMMMMMMMMMMMMMNNNNNNNmd"
	@echo "NNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNy/+symh/-------::////:+hNMMMMMMMMMMMMMMMNNNNNNNmd"
	@echo "NNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMm+oyhhdh+--------------:-:/hNMMMMMMMMMMMMMNNNNNNNNN"
	@echo "NNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNdsmNyo/-------------..-----::odMMMMMMMMMMMMNNNNNNNMM"
	@echo "NNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMdss:-/+s+:------------....--..--:/hMMMMMMMMMMMNNNNNNNMM"
	@echo "NMMMMMMMMMMMMMMMMMMMMMMMMMMMMmddmmmNNNNNNMMNs:------------..--........-.------::hMMMMMMMMMMNNNNNNNNM"
	@echo "NMMMMMMMMMMMMMMMMMMMMMMMMMMMN-...-:/+++++oo+:----------..................------:/dMMMMMMMMMNNNNNNNNM"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMN/     '.-:::::---..................................-+NMMMMMMMMNNNNNNNNN"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMNh/-.'  '.-----....--............''.................-dMMMMMMMMNNNNNNNmm"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNmdhysooo/-....-----..........'..................-sMMMMMMMMNNNNNmNmd"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMho+/:-----.--.......'''..............'''./NMMMMMMMNNNNNmNNm"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMdshyo//:-...........'' '''....'..........+MMMMMMMMNNNNNmNNN"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMdyyo+////::-://++//:-.'   ''''.......''.yMMMMMMMMNNNNmmNNN"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMdhsso/-/oosssssyyyys+.''     ''''..'''-mMMMMMMMMNNNmmmNmm"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdyyyys+ssoosyhhhhhhho-'''''''.::-''...sMMMMMMMMMNNmmmmmmm"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdhhhdddddhhhhhddddddho-....--://:+:'''-mMMMMMMMMMNNmmmmmmd"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdhyyhddmddddddddddhhhyo:--://:/+o///''':mMMMMMMMMMNNmmmmmmh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdhysyyshdmmdddddhhhhyys//-/os/:/+o/:''''.sNNMMMMMMMNNmmmmmmh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmhysoo+ydmmddhhhhhhhhyo:/+osossoo+-' '' '+mNNMMMMMMNNmmmmmmh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNhyyhdddddddhhhhhhhhs+oo+oshhs+/.''   ''sNNNmNMMMNNmmddmmh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdddhhdddddhhhhhhhhyyso+/+oo+++/-'   '-./oymNMMMMNmmdddmdh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNhysssshhhdhhhhhhhhyyo+++/++++++:.  '../::ymNMMMMNmddddmdh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdddhddddhhhhhhyyysso+//++++++o+/:.--.-/yy+oydmmNNmddddmdh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNmdo:/++:hdhyyyyhyyyyysso+////+++++++ooo++oos/-oo::++/osyyydddmmd"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNmyso++-'-:-'odhhhddhysyso+/////+++++++++++o+oshhso+:::::://:-/oo+shh"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMMMMNdyo/://::.'.-- '+ossso+/::--:://///++++++++oo+osyyyo+:-:::--::--::::+oo"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMMMNdy+/+:-.-::-. '.. '.'....-'-/.-:///+++++++++o+++++oss+/:-:::--::-----:/+++"
	@echo "MMMMMMMMMMMMMMMMMMMMMMMmyo+/:-::-..--.' '.-' .''...:..//::://+/+/+++oosoo++o+////:--:--::-----:/++++"
	@echo "MMMMMMMMMMMMMMMMMMMMMNhoo+::-..:-..--..'''.' ..''...:-.:+/:::::/++++oosoooo+////::-----:-..-----://:"
	@echo "MMMMMMMMMMMMMMMMMMMMmy+///-....::......'''.' '.''....::--://////+++++oooo+/:/+/-------:--.---:--::::"
	@echo "MMMMMMMMMMMMMMMMMMMdo+/-:/:.''.-:-.....'''.' '..''....-:-:::---::://+++//+ooo/....--::--..------::-:"
	@echo "MMMMMMMMMMMMMMMMMNho+//:--:-.'..--.....' '.' ........'...--:::::://++ooooo+:-....-----.....---..::--"
	@echo "MMMMMMMMMMMMMMMMNh+//:-:-.--.''.....'.'' ..' '.....'''......-:////+++++++:--...-------.-...--...---."
	@echo "MMMMMMMMMMMMMMMNyo/:::----..-.'...'..''  ..' '....'''''''...'.-//+/++o+:-.....----------...--...--.."
	@echo "MMMMMMMMMMMMMMMd/:::-------.'-....''..'  '.' '''..'''..'''.....-:////:-......----.------...--......-"
	@echo "MMMMMMMMMMMMMMNs/:------..-.''..--.'''' ''.' '''...'...'''......-::--.......----..------..--.''..'.-"
	@echo "MMMMMMMMMMMMMMd+::-------....''.... ''''''..''''''...'''''.'''....-........-......----.....'.''.'..."
	@echo "${GREEN}Enjoy Hashgraph ^.^"
