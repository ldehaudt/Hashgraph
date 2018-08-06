#ifndef HASHGRAPHS_HPP
#define HASHGRAPHS_HPP
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include <sstream>
#include "Event.hpp"


const int N = 4; //number of members in the entire population (n > 1)
const int C = 5; // a small integer constant greater than 2, such as c = 10

// KILLLLLLLLLL MMEEEEEEEEEEEEE
extern int testingNum;

class Person;
typedef struct s_data data;

extern int runTime;
extern std::array<Person*, N> people;
std::string		md5_hash(std::string str);

#endif