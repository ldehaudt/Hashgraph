#include "Event.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include "Hashgraphs.hpp"

Person pdb;

Event::Event(Person &p, Event *self, Event *gossiper, unsigned long t)
: selfParent(self), owner(p), gossiperParent(gossiper), timestamp(t) {
	if (selfParent)
	{
		std::cout << "SelfParent " << selfParent->tVal << std::endl;
		std::cout << "GossiperParent " << gossiperParent->tVal << std::endl;
	}
	roundRecieved = -1;
	consensusTimestamp = -1;
	famous = -1;
	witness = false;
	vote = false;
	tVal = testingNum;
	testingNum++;
}

Event::Event() : owner(pdb) {

}
Event::~Event(){}
Event::Event(const Event &rhs) : owner(rhs.owner) {
	*this = rhs;
}

Event & Event::operator=(const Event &rhs){
	owner = rhs.getOwner();
    if (rhs.getSelfParent())
		selfParent = rhs.getSelfParent();
	else
		selfParent = NULL;
    //PAYLOAD  MISSING
    if (rhs.getGossiperParent())
		gossiperParent = rhs.getGossiperParent();
	else
		gossiperParent = NULL;
	consensusTimestamp = rhs.getConsensusTimestamp();
	timestamp = rhs.getTimestamp();
	roundRecieved = rhs.getRoundRecieved();
	round = rhs.getRound();
	witness = rhs.getWitness();
	famous = rhs.getFamous();
	vote = rhs.getVote();
	tVal = rhs.tVal; //KILLLLLLLLL MEMEMEMEMEMEMEMEMEMEME
	return (*this);
}

void Event::divideRounds(){
	if (!this->selfParent)
	{
		round = 0;
		return;
	}
	round = this->selfParent->getRound();
	if (this->gossiperParent->getRound() > round)
		round = this->gossiperParent->getRound();
	int numStrongSee = 0;
	std::vector<Event> witnesses = owner.findWitnesses(round);    
	for (int i = 0; i < static_cast<int>(witnesses.size()); i++){
		if (stronglySee(witnesses[i]))
			numStrongSee++;
	}
	if (numStrongSee > 2 * N / 3)
		round = round + 1;
	witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

bool Event::operator==(Event &rhs){
	return (timestamp == rhs.getTimestamp() && owner == rhs.getOwner());
}

bool Event::seeRecursion(Event y, std::vector<Event> *forkCheck){
	if (this->round < y.getRound())
		return false;
	if (this->getOwner() == y.getOwner())
		(*forkCheck).push_back(*this);
	if (*this == y)
		return true;
	return this->getSelfParent()->seeRecursion(y, forkCheck) ||
	this->getGossiperParent()->seeRecursion(y, forkCheck);
}

bool Event::see(Event y){
	//fork stops when we reach y, might need to be changed !!
	std::vector<Event> forkCheck;
	bool b = seeRecursion(y, &forkCheck);
	for (unsigned int i = 0; i < forkCheck.size(); i++)
		for (unsigned int j = i + 1; j < forkCheck.size(); j++)
			if (fork(forkCheck[i],forkCheck[j]))
				return false;
	return b;
}

bool Event::stronglySee(Event y){
	int numSee = 0;
	int i;
	std::vector<Person*> found;
	// std::list<Event>::iterator iter;
	for (unsigned int n = 0; n < owner.getHashgraph().size(); n++)
	{
		for (i = 0; i < numSee; i++)
			if (*(found[i]) == owner)
				break ;
			if (i == numSee)
				if (see(owner.getHashgraph()[n]) && owner.getHashgraph()[n].see(y))
				{
					numSee++;
					found.push_back(&(owner.getHashgraph()[n].getOwner()));
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

Event  *Event::getSelfParent() const {    
	return (selfParent);
}
Person &Event::getOwner() const {    
	return (owner);
}
Event  *Event::getGossiperParent() const {    
	return (gossiperParent);
}
unsigned long   Event::getTimestamp() const {    
	return (timestamp);
}
int     Event::getRound() const {    
	return (round);
}
bool    Event::getWitness() const {    
	return (witness);
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
bool    Event::getVote() const {    
	return (vote);
}
void    Event::setFamous(char fame){
	famous = fame;
}
void    Event::setVote(bool b){
	vote = b;
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
