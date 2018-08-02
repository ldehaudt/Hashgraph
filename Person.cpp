#include "Person.hpp"
#include <list>
#include <vector>

Event::Event(Person &p, Event *self, Event *gossiper) : selfParent(self), owner(p), gossiperParent(gossiper) {

}

bool Event::operator==(Event &rhs){
    return (owner == rhs.getOwner() && timestamp == rhs.getTimestamp());
}

bool Event::seeRecursion(Event* y, std::vector<Event*> *forkCheck){
    if (!this || this->round < y->getRound())
        return false;
    if (this->getOwner() == y->getOwner()){
        (*forkCheck).push_back(this);
    }
    if (this == y)
        return true;
    return this->getSelfParent()->seeRecursion(y, forkCheck) || this->getGossiperParent()->seeRecursion(y, forkCheck);
}

bool Event::see(Event* y){
    //fork stops when we reach y, might need to be changed !!
    std::vector<Event*> forkCheck;
    bool b = seeRecursion(y, &forkCheck);
    for (int i = 0; i < forkCheck.size() - 1; i++){
        for (int j = i + 1; j < forkCheck.size(); j++){
            if (fork(*(forkCheck[i]),*(forkCheck[j])))
                return false;
        }
    }
    return b;
}

bool Event::stronglySee(Event* y){
    int numSee = 0;
    int i;
    std::vector<Person*> found;
    std::list<Event*>::iterator iter;

    for (iter = owner.getHashgraph().begin(); iter != owner.getHashgraph().end(); iter++){
        for (i = 0; i < numSee; i++)
            if (*(found[i]) == owner)
                break ;
        if (i == numSee)
            if (see(*iter) && (*iter)->see(y))
            {
                numSee++;
                found.push_back(&((*iter)->getOwner()));
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

#pragma region get/set
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
    vote = b
}

#pragma endregion

std::vector<Event*> Person::findWitnesses(int round){ //probably need player whose graph to look into as param 
    std::list<Event*>::iterator iter;
    std::vector<Event*> witnesses;

    for (iter = getHashgraph().begin(); (*iter)->getRound() >= round - 1; iter++){
        if ((*iter)->getRound() == round && (*iter)->getWitness() == true){
            witnesses.push_back(*iter);
        }
    }
    return witnesses;
}

void Event::divideRounds(){
    round = this->selfParent->getRound();
    if (this->gossiperParent->getRound() > round)
        round = this->gossiperParent->getRound();
    int numStrongSee = 0;
    std::vector<Event*> witnesses = owner.findWitnesses(round);
    
    for (int i = 0; i < witnesses.size(); i++){
        if (stronglySee(witnesses[i]))
            numStrongSee++;
    }
    if (numStrongSee > 2 * N / 3)
        round = round + 1;
    witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

void Person::decideFame(){ //probably need player whose graph to look into as param
    std::list<Event*>::iterator x;
    std::list<Event*>::iterator y;
    int d;
    std::vector<Event*> s;
    int v;
    int t;

    for (x = hashgraph.end() - 1; x != hashgraph.begin() - 1; x--){ // p is player (PREV COMMENT)
        (*x)->setFamous(-1); //-1 undefined, 0 false, 1 true
        for (y = hashgraph.end() - 1; y != hashgraph.begin() - 1; y--){
            if ((*x)->getFamous() && (*y)->getFamous() && (*y)->getRound() > (*x)->getRound()){
                d = (*y)->getRound() - (*x)->getRound();
                s = findWitnesses((*y)->getRound() - 1);
                for (int i = 0; i < s.size(); i++){
                                      
                }
                v = ;
                t = ;
                if (d == 1){
                    (*y)->setFamous((*y)->see(*x));
                }else{
                    if (d % C > 0){
                        if (t > 2 * N / 3){
                            (*x)->famous = v;
                            (*y)->vote = v;
                            break;
                        }
                        else{
                            (*y)->setVote(v);
                        }
                    }
                    else{
                        if (t > 2 * N / 3){
                            (*y)->setVote(v);
                        }else{
                            y.vote = ;//COIN TOSS SHOULD BE UNIFORM ??
                        }
                    }
                }
            }
        }
    }
}

void Person::findOrder(){

}


void Person::gossip(Person &p){
    p.recieveGossip(*this);
}

Event *Person::getTopNode(Person &p, Person &target){
    std::list<Event*>::iterator iter;

    for (iter = p.hashgraph.begin(); iter != p.hashgraph.end(); iter++){
        if ((*iter)->getOwner() == target){
            return (*iter);
        }
    }
    return NULL;
}

void Person::createEvent(time_t time, Person &gossiper){
    Event *tmp = new Event(*this, Person::getTopNode(*this, *this), Person::getTopNode(*this, gossiper));
    hashgraph.push_front(tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event*> gossip){
    std::list<Event*>::iterator iter;
    std::list<Event*>::iterator tmp;
    bool self;
    bool gos;

    for (int i = 0; i < gossip.size(); i++){
        self = false;
        gos = false;
        for (iter = hashgraph.begin(); iter != hashgraph.end(); iter++){
            if (*iter == gossip[i]->getSelfParent())
                self = true;
            if (*iter == gossip[i]->getGossiperParent())
                gos = true;
            if (self && gos)
            {

                for (tmp = hashgraph.begin(); tmp != hashgraph.end(); tmp++){
                    if ((*tmp)->getRound() == newEvent->getRound())
                }
                break;
            }
        }
    }
    createEvent(time(0) - startTime, gossiper);    
}

bool Person::operator==(Person &rhs){
    return index == rhs.index;
}

std::list<Event*>    Person::getHashgraph(){
    return hashgraph;
}

