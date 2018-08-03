#include "Person.hpp"
#include <ctime>
#include "Hashgraphs.hpp"
#include <array>
#include <iterator>
#include <unistd.h>

time_t startTime;
int testingNum = 1000;

void    printHash(Person* p){
    std::cout << "Printing person # " << p->index << "\n";
    for (unsigned int i = 0; i < p->getHashgraph().size(); i++){
        std::cout << p->getHashgraph()[i].tVal << "; " << p->getHashgraph()[i].getTimestamp() << "\n";
    }
}

int main(){
    std::array<Person*, N> people;
    srand(time(NULL));

    startTime = time(0);
    for (int i = 0; i < N; i++)
        people[i] = new Person(i);
    while (1)
    {
        int i = std::rand() % N;
std::cout << i << std::endl;
        int j;
        while ((j = std::rand() % N) == i)
            ;
std::cout << j << std::endl;
        people[i]->gossip(*(people[j]));
        printHash(people[j]);
        sleep(1);
    }
}
