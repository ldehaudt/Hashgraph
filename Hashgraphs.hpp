#ifndef HASHGRAPHS_HPP
#define HASHGRAPHS_HPP

#include <ctime>
#include <list>
#include <vector>
#include "Event.hpp"
#include "Person.hpp"

const int N = 4; //number of members in the entire population (n > 1)
const int C = 10; // a small integer constant greater than 2, such as c = 10

class Person;

extern time_t startTime;
extern std::vector<Person*> people;

#endif