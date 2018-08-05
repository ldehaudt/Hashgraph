#include "Event.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include "Hashgraphs.hpp"

Person pdb;

Event::Event(Person &p, int selfHash, int gossipHash, unsigned long t) //self and 
: owner(p) {
	round = 0;
	d.timestamp = t;
	d.selfP = selfHash;
	d.gossipP = gossipHash;
	d.owner = owner.index;
	gossiperParent = NULL;
	selfParent = NULL;
	roundRecieved = -1;
	consensusTimestamp = -1;
	famous = -1;
	witness = (selfParent ? false : true);
	vote = false;
	tVal = testingNum;
	testingNum++;
	hash = tVal;
}

Event::Event() : owner(pdb) {

}
Event::~Event(){}
Event::Event(const Event &rhs) : owner(rhs.owner) {
	*this = rhs;
}

Event & Event::operator=(const Event &rhs){
	owner = rhs.getOwner();
    d.timestamp = rhs.getData().timestamp;
	d.selfP = rhs.getData().selfP;
	d.gossipP = rhs.getData().gossipP;
	d.owner = rhs.getData().owner;
    //PAYLOAD  MISSING
	gossiperParent = NULL;
	selfParent = NULL;
	consensusTimestamp = rhs.getConsensusTimestamp();
	roundRecieved = rhs.getRoundRecieved();
	round = rhs.getRound();
	witness = rhs.getWitness();
	famous = rhs.getFamous();
	vote = rhs.getVote();
	tVal = rhs.tVal; //KILLLLLLLLL MEMEMEMEMEMEMEMEMEMEME
	hash = tVal; //change to md5 once that works
	return (*this);
}

void Event::divideRounds(){
	if (!this->selfParent)
	{
		round = 0;
		return;
	}
	round = this->selfParent->getRound();
std::cout << "Parent: owner: " << this->selfParent->getOwner().index << " ; time: "
<< this->selfParent->getData().timestamp << " round: " << this->selfParent->getRound() << std::endl;
std::cout << "GossiperParent: owner: " << this->gossiperParent->getOwner().index << " ; time: "
<< this->gossiperParent->getData().timestamp << " round: " << this->gossiperParent->getRound() << std::endl;
	if (this->gossiperParent->getRound() > round)
		round = this->gossiperParent->getRound();
	int numStrongSee = 0;
	std::vector<Event*> witnesses = owner.findWitnesses(round);
std::cout << "here?\n";
	for (unsigned int i = 0; i < witnesses.size(); i++)
		if (stronglySee(witnesses[i]))
			numStrongSee++;
	if (numStrongSee > 2 * N / 3)
		round = round + 1;
std::cout << "owner: " << owner.index << " ; time: "
<< d.timestamp << " round: " << round << std::endl;
	witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

bool Event::operator==(Event &rhs){
	return (hash == rhs.getHash()); // change once actualt hash used
}

bool Event::seeRecursion(Event *y, std::vector<Event*> *forkCheck){
	if (this->round < y->getRound())
		return false;
	if (this->getOwner() == y->getOwner())
		(*forkCheck).push_back(this);
	if (this == y)
		return true;
	if (!this->getSelfParent())
		return false;
	return this->getSelfParent()->seeRecursion(y, forkCheck) ||
	this->getGossiperParent()->seeRecursion(y, forkCheck);
}

bool Event::see(Event *y){
	//fork stops when we reach y, might need to be changed !!
	std::vector<Event*> forkCheck;
	bool b = seeRecursion(y, &forkCheck);
	for (unsigned int i = 0; i < forkCheck.size(); i++)
		for (unsigned int j = i + 1; j < forkCheck.size(); j++)
			if (fork(forkCheck[i],forkCheck[j]))
				return false;
	return b;
}

bool Event::stronglySee(Event *y){
    int numSee = 0;
    std::array<bool, N> found = {false};

    for (unsigned int n = 0; n < owner.getHashgraph().size(); n++)
    {
        if (found[owner.getHashgraph()[n]->getOwner().index] == true)
            continue ;
        if (see(owner.getHashgraph()[n]) && owner.getHashgraph()[n]->see(y))
        {
            numSee++;
            found[owner.getHashgraph()[n]->getOwner().index] = true;
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

Event  *Event::getSelfParent() const {    
	return (selfParent);
}
Person &Event::getOwner() const {    
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
