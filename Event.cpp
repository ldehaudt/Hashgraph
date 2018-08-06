#include "Event.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include "Hashgraphs.hpp"

<<<<<<< HEAD
Event::Event(Person &p, data data) {
	graph = p.getHashgraph();
	round = 0;
	d = data;
=======
Event::Event(int p, int selfHash, int gossipHash, int t) //self and 
: owner(p) {
	graph = people[p]->getHashgraph();
	round = 0;
	d.timestamp = t;
	d.selfP = selfHash;
	d.gossipP = gossipHash;
	d.owner = p;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	gossiperParent = NULL;
	selfParent = NULL;
	roundRecieved = -1;
	consensusTimestamp = -1;
	famous = -1;
<<<<<<< HEAD
	witness = (d.selfP ? false : true);
	hash = d.tVal;
=======
	witness = (selfParent ? false : true);
	tVal = testingNum;
	testingNum++;
	hash = tVal;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
}

Event::Event(){}
Event::~Event(){}
Event::Event(const Event &rhs){
	*this = rhs;
}

Event & Event::operator=(const Event &rhs){
	graph = rhs.getGraph();
<<<<<<< HEAD
    d = rhs.getData();
=======
	owner = rhs.getOwner();
    d.timestamp = rhs.getData().timestamp;
	d.selfP = rhs.getData().selfP;
	d.gossipP = rhs.getData().gossipP;
	d.owner = rhs.owner;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
    //PAYLOAD  MISSING
	gossiperParent = NULL;
	selfParent = NULL;
	consensusTimestamp = rhs.getConsensusTimestamp();
	roundRecieved = rhs.getRoundRecieved();
	round = rhs.getRound();
	witness = rhs.getWitness();
	famous = rhs.getFamous();
<<<<<<< HEAD
	hash = d.tVal; //change to md5 once that works
=======
	tVal = rhs.tVal; //KILLLLLLLLL MEMEMEMEMEMEMEMEMEMEME
	hash = tVal; //change to md5 once that works
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	return (*this);
}

void Event::divideRounds(){
	if (!this->selfParent || !this->gossiperParent)
	{
		round = 0;
		return;
	}
	round = this->selfParent->getRound();
	if (this->gossiperParent->getRound() > round)
		round = this->gossiperParent->getRound();
	int numStrongSee = 0;
<<<<<<< HEAD
	std::vector<Event*> witnesses = people[d.owner]->findWitnesses(round);
=======
	std::vector<Event*> witnesses = people[owner]->findWitnesses(round);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	for (unsigned int i = 0; i < witnesses.size(); i++)
		if (stronglySee(witnesses[i]))
			numStrongSee++;
	if (numStrongSee > 2 * N / 3)
		round = round + 1;
std::cout << "round " << round << std::endl;
	witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

bool Event::operator==(Event &rhs){
	return (hash == rhs.getHash()); // change once actualt hash used
}

bool Event::seeRecursion(Event *y, std::vector<Event*> *forkCheck, bool *done, std::vector<Event*> *visited){
    if (std::find((*visited).begin(),(*visited).end() , this) != (*visited).end())
    	return (*this == *y);
    visited->push_back(this);
    if (*done)
        return true;
    if (*this == *y)
    {
        return true;
        *done = true;
    }
    if (d.timestamp < y->getData().timestamp)
        return false;
<<<<<<< HEAD
    if (d.owner == y->getData().owner)
=======
    if (this->getOwner() == y->getOwner())
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
        (*forkCheck).push_back(this);
    if (!this->getSelfParent())
        return false;
    return this->getSelfParent()->seeRecursion(y, forkCheck, done, visited) ||
    this->getGossiperParent()->seeRecursion(y, forkCheck, done, visited);
}

bool Event::see(Event *y){
    std::vector<Event*> forkCheck;
    std::vector<Event*> visited;
    bool done = false;
    bool b = seeRecursion(y, &forkCheck, &done, &visited);
    for (unsigned int i = 0; i < forkCheck.size(); i++)
        for (unsigned int j = i + 1; j < forkCheck.size(); j++)
            if (fork(forkCheck[i],forkCheck[j]))
                return false;
    return b;
}

bool Event::ancestorRecursion(Event *y, bool* done, std::vector<Event*> *visited){
	if (std::find((*visited).begin(),(*visited).end() , this) != (*visited).end())
    	return (*this == *y);
    visited->push_back(this);
	if (*done)
        return true;
    if (*this == *y)
    {
        return true;
        *done = true;
    }
    if (d.timestamp < y->getData().timestamp)
        return false;
    if (!this->getSelfParent())
        return false;
    return (this->getSelfParent()->see(y)) || (this->getGossiperParent()->see(y));
}

bool Event::ancestor(Event *y){
	std::vector<Event*> visited;
	bool done = false;
	return ancestorRecursion(y, &done, &visited);
}

void Event::decideFame(){
	std::vector<Event*> s;
	unsigned int count;
	unsigned int countNo;
	bool v;
	int d;

	if (!witness || round < 2)
		return ;
<<<<<<< HEAD
=======
std::cout << std::endl;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	for (unsigned int x = graph->size() - 1; x < graph->size(); x--)
	{
		if (!((*graph)[x]->getWitness()) || (*graph)[x]->getFamous() != -1
			|| (*graph)[x]->getRound() > round - 2)
			continue;
<<<<<<< HEAD
		s = people[getData().owner]->findWitnesses((*graph)[x]->getRound() + 1);
=======
		s = people[owner]->findWitnesses((*graph)[x]->getRound() + 1);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
		count = 0;
		countNo = 0;
		for (unsigned int y = 0; y < s.size(); y++)
		{
			if (!stronglySee(s[y]))
			{
				s.erase(s.begin() + y);
				continue ;
			}
			if (s[y]->see((*graph)[x]))
				count++;
			else
				countNo++;
		}
		d = round - (*graph)[x]->getRound();
		if (count > 2 * N / 3)
			(*graph)[x]->setFamous(1);
		else if (countNo > 2 * N / 3)
			(*graph)[x]->setFamous(0);
		else if (!(d % C))
<<<<<<< HEAD
			(*graph)[x]->setFamous((*graph)[x]->getHash() % 2);
=======
		{
std::cout << "COIN ROUND : " << (*graph)[x]->tVal;
			(*graph)[x]->setFamous((*graph)[x]->tVal % 2);
std::cout << ((*graph)[x]->getFamous() ? " Famous" : " Not Famous") << std::endl;
		}
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	}
}

bool Event::stronglySee(Event *y){
    int numSee = 0;
    std::array<bool, N> found = {false};

    for (unsigned int n = 0; n < graph->size(); n++)
    {
<<<<<<< HEAD
        if (found[(*graph)[n]->getData().owner] == true || (*graph)[n]->getRound() < y->getRound() )
=======
        if (found[(*graph)[n]->getOwner()] == true || (*graph)[n]->getRound() < y->getRound() )
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
            continue ;
        if (this->see((*graph)[n]) && (*graph)[n]->see(y))
        {
            numSee++;
<<<<<<< HEAD
            found[(*graph)[n]->getData().owner] = true;
=======
            found[(*graph)[n]->getOwner()] = true;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
            if (numSee > 2 * N / 3)
                return true;
        }
    }
    return false;
}

bool Event::fork(Event *x, Event *y){
	Event *t;

	if (!(x->getData().owner == y->getData().owner))
		return 0;
	t = x;
	while (t)
	{
		if (t == y)
			return 0;
		t = t->getSelfParent();
	}
	t = y;
	while (t)
	{
		if (t == x)
			return 0;
		t = t->getSelfParent();
	}
	return 1;
}

std::vector<Event*> *Event::getGraph() const {
	return (graph);
}

Event  *Event::getSelfParent() const {    
	return (selfParent);
}
<<<<<<< HEAD
=======
int Event::getOwner() const {    
	return (owner);
}
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
Event  *Event::getGossiperParent() const {    
	return (gossiperParent);
}
data   Event::getData() const{
	return d;
}
int     Event::getRound() const {    
	return (round);
}
bool    Event::getWitness() const {    
	return (witness);
}
int		Event::getHash() const{
	return hash;
}
int  Event::getConsensusTimestamp() const {    
	return (consensusTimestamp);
}   
int     Event::getRoundRecieved() const {    
	return (roundRecieved);
}
char    Event::getFamous() const {    
	return (famous);
}
void    Event::setFamous(char fame){
	famous = fame;
}
void    Event::setRoundReceived(int r) {
	roundRecieved = r;
}
void    Event::setConsensusTimestamp(int t) {
	consensusTimestamp = t;
}
void	Event::setSelfParent(Event *e) {
	selfParent = e;
}
void	Event::setGossiperParent(Event *e) {
	gossiperParent = e;
}
