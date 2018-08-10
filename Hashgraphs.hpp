#ifndef HASHGRAPHS_HPP
# define HASHGRAPHS_HPP
# include <SDL2/SDL.h>
# include <time.h>
# include <iostream>
# include <vector>
# include <array>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include "Event.hpp"

# define W 1000
# define H 1300
# define M 150
# define GAP 60 / N

const int	N = 6;
const int	C = 5;

class					Person;
typedef struct s_data	data;

extern int						runTime;
extern std::array<Person*, N>	people;

std::string		md5_hash(std::string str);

#endif
