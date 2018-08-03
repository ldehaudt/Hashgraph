#include "Person.hpp"
#include "Event.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>

Event::Event(Person &p, Event *self, Event *gossiper, double t) : selfParent(self), owner(p), gossiperParent(gossiper), timestamp(t) {
    roundRecieved = -1;
    consensusTimestamp = -1;
    famous = -1;
    witness = false;
    vote = false;
}

Event::Event(){}
Event::~Event(){}
Event::Event(Event &rhs) {
    *this = rhs;
}

Event & Event::operator=(Event &rhs){
    selfParent = rhs.getSelfParent();
    owner = rhs.getOwner();
    //PAYLOAD  MISSING
    gossiperParent = rhs.getGossiperParent();
    consensusTimestamp = rhs.getConsensusTimestamp();
    timestamp = rhs.getTimestamp();
    roundRecieved = rhs.getRoundRecieved();
    round = rhs.getRound();
    witness = rhs.getWitness();
    famous = rhs.getFamous();
    vote = rhs.getVote();

}

bool Event::operator==(Event &rhs){
    return (owner == rhs.getOwner() && timestamp == rhs.getTimestamp());
}

bool Event::seeRecursion(Event y, std::vector<Event> *forkCheck){
    if (!this || this->round < y.getRound())
        return false;
    if (this->getOwner() == y.getOwner()){
        (*forkCheck).push_back(*this);
    }
    if (*this == y)
        return true;
    return this->getSelfParent()->seeRecursion(y, forkCheck) || this->getGossiperParent()->seeRecursion(y, forkCheck);
}

bool Event::see(Event y){
    //fork stops when we reach y, might need to be changed !!
    std::vector<Event> forkCheck;
    bool b = seeRecursion(y, &forkCheck);
    for (int i = 0; i < forkCheck.size() - 1; i++){
        for (int j = i + 1; j < forkCheck.size(); j++){
            if (fork(forkCheck[i],forkCheck[j]))
                return false;
        }
    }
    return b;
}

bool Event::stronglySee(Event y){
    int numSee = 0;
    int i;
    std::vector<Person*> found;
    std::list<Event>::iterator iter;

    for (iter = owner.getHashgraph().begin(); iter != owner.getHashgraph().end(); iter++){
        for (i = 0; i < numSee; i++)
            if (*(found[i]) == owner)
                break ;
        if (i == numSee)
            if (see(*iter) && iter->see(y))
            {
                numSee++;
                found.push_back(&(iter->getOwner()));
                if (numSee > 2 * N / 3)
                    return true;
            }
    }
    return false;
}

bool Event::fork(Event& x, Event& y){
    Event *t;

    if (!(x.getOwner() == y.getOwner()))
        return 0;
    t = &x;
    while (t)
    {
        if (*t == y)
            return 0;
        t = t->getSelfParent();
    }
    t = &y;
    while (t)
    {
        if (*t == x)
            return 0;
        t = t->getSelfParent();
    }
    return 1;
}

Event  *Event::getSelfParent(){    
    return (selfParent);
}
Person &Event::getOwner(){    
    return (owner);
}
Event  *Event::getGossiperParent(){    
    return (gossiperParent);
}
double  Event::getTimestamp(){    
    return (timestamp);
}
int     Event::getRound(){    
    return (round);
}
bool    Event::getWitness(){    
    return (witness);
}
double  Event::getConsensusTimestamp(){    
    return (consensusTimestamp);
}   
int     Event::getRoundRecieved(){    
    return (roundRecieved);
}
char    Event::getFamous(){    
    return (famous);
}
bool    Event::getVote(){    
    return (vote);
}
void    Event::setFamous(char fame){
    famous = fame;
}
void    Event::setVote(bool b){
    vote = b;
}
