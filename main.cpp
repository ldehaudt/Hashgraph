#include "Person.hpp"
#include <ctime>
#include "Hashgraphs.hpp"
#include <array>
#include <iterator>

time_t startTime;
int testingNum = 1000;

void    printHash(Person* p){
    std::cout << "Printing person # " << p->index << "\n";
std::cout << p->getHashgraph().size() << "\n";
    for (unsigned int i = 0; i < p->getHashgraph().size(); i++){
        std::cout << p->getHashgraph()[i].tVal << "\n";
    }
}

int main(){
    std::array<Person*, N> people;

    startTime = time(0);      
    for (int i = 0; i < N; i++)
        people[i] = new Person(i);

    people[2]->gossip(*(people[1]));
    printHash(people[1]);
    people[2]->gossip(*(people[1]));
   //printHash(people[0]);
    printHash(people[1]);
}
