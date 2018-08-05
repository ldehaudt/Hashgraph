#include "Event.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include "Hashgraphs.hpp"

Event::Event(int p, int selfHash, int gossipHash, unsigned long t) //self and 
: owner(p) {
	graph = people[p]->getHashgraph();
	round = 0;
	d.timestamp = t;
	d.selfP = selfHash;
	d.gossipP = gossipHash;
	d.owner = p;
	gossiperParent = NULL;
	selfParent = NULL;
	roundRecieved = -1;
	consensusTimestamp = -1;
	famous = -1;
	witness = (selfParent ? false : true);
	tVal = testingNum;
	testingNum++;
	hash = tVal;
}

Event::Event(){}
Event::~Event(){}
Event::Event(const Event &rhs){
	*this = rhs;
}

Event & Event::operator=(const Event &rhs){
	graph = rhs.getGraph();
	owner = rhs.getOwner();
    d.timestamp = rhs.getData().timestamp;
	d.selfP = rhs.getData().selfP;
	d.gossipP = rhs.getData().gossipP;
	d.owner = rhs.owner;
    //PAYLOAD  MISSING
	gossiperParent = NULL;
	selfParent = NULL;
	consensusTimestamp = rhs.getConsensusTimestamp();
	roundRecieved = rhs.getRoundRecieved();
	round = rhs.getRound();
	witness = rhs.getWitness();
	famous = rhs.getFamous();
	tVal = rhs.tVal; //KILLLLLLLLL MEMEMEMEMEMEMEMEMEMEME
	hash = tVal; //change to md5 once that works
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
	std::vector<Event*> witnesses = people[owner]->findWitnesses(round);
	for (unsigned int i = 0; i < witnesses.size(); i++)
		if (stronglySee(witnesses[i]))
			numStrongSee++;
	if (numStrongSee > 2 * N / 3)
		round = round + 1;
	witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

bool Event::operator==(Event &rhs){
	return (hash == rhs.getHash()); // change once actualt hash used
}

// bool Event::seeRecursion(Event *y, std::vector<Event*> *forkCheck){
// 	if (this->round < y->getRound())
// 		return false;
// 	if (this->getOwner() == y->getOwner())
// 		(*forkCheck).push_back(this);
// 	if (this == y)
// 		return true;
// 	if (!this->getSelfParent())
// 		return false;
// 	return this->getSelfParent()->seeRecursion(y, forkCheck) ||
// 	this->getGossiperParent()->seeRecursion(y, forkCheck);
// }

bool Event::see(Event *y){
	if (this->round < y->getRound())
		return false;
	if (*this == *y)
		return true;
	if (!this->getSelfParent())
		return false;
	return (this->getSelfParent()->see(y)) || (this->getGossiperParent()->see(y));
}

// bool Event::see(Event *y){
// 	//fork stops when we reach y, might need to be changed !!
// 	std::vector<Event*> forkCheck;
// 	bool b = seeRecursion(y, &forkCheck);
// 	for (unsigned int i = 0; i < forkCheck.size(); i++)
// 		for (unsigned int j = i + 1; j < forkCheck.size(); j++)
// 			if (fork(forkCheck[i],forkCheck[j]))
// 				return false;
// 	return b;
// }

void Event::decideFame(){
	std::vector<Event*> s;
	unsigned int count;
	unsigned int countNo;
	bool v;
	int d;

	if (!witness || round < 2)
		return ;
	for (unsigned int x = graph->size() - 1; x < graph->size(); x--)
	{
		if (!((*graph)[x]->getWitness()) || (*graph)[x]->getFamous() != -1
			|| (*graph)[x]->getRound() > round - 2)
			continue;
		s = people[owner]->findWitnesses((*graph)[x]->getRound() + 1);
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
			(*graph)[x]->setFamous((*graph)[x]->tVal % 2);
	}
}

bool Event::stronglySee(Event *y){
    int numSee = 0;
    std::array<bool, N> found = {false};

    for (unsigned int n = 0; n < graph->size(); n++)
    {
        if (found[(*graph)[n]->getOwner()] == true)
            continue ;
        if (this->see((*graph)[n]) && (*graph)[n]->see(y))
        {
            numSee++;
            found[(*graph)[n]->getOwner()] = true;
            if (numSee > 2 * N / 3)
                return true;
        }
    }
    return false;
}

bool Event::fork(Event *x, Event *y){
	Event *t;

	if (!(x->getOwner() == y->getOwner()))
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
int Event::getOwner() const {    
	return (owner);
}
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
unsigned long  Event::getConsensusTimestamp() const {    
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
void    Event::setConsensusTimestamp(unsigned long t) {
	consensusTimestamp = t;
}
void	Event::setSelfParent(Event *e) {
	selfParent = e;
}
void	Event::setGossiperParent(Event *e) {
	gossiperParent = e;
}
