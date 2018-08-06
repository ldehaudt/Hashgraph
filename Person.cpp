#include "Person.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>

std::vector<Event*> Person::findWitnesses(int round){
	std::vector<Event*> witnesses;
	for (unsigned int i = 0; i < getHashgraph()->size()
		&& (*getHashgraph())[i]->getRound() >= round - 1; i++)
		if ((*getHashgraph())[i]->getRound() == round
			&& (*getHashgraph())[i]->getWitness() == true)
			witnesses.push_back((*getHashgraph())[i]);
	return witnesses;
}

Person::Person(){}
Person::~Person(){}
Person::Person(Person &rhs){
	*this = rhs;    
}

Person & Person::operator=(Person &){
	return *this;
}

Person::Person(int ind) : index(ind) {
	currentRound = 0;
	data d;
	d.owner = index;
	d.selfP = "";
	d.gossipP = "";
	d.timestamp = runTime;
	Event* tmp = new Event(*this, d);
	hashgraph.insert(hashgraph.begin(), tmp);
}

std::array<Event*, N> findUFW(std::vector<Event*> witnesses){
	std::array<Event*, N> arr;
	for (int i = 0; i < N; i++)
		arr[i] = NULL;
	char b[N] = { 0 };

	for(unsigned int i = 0; i < witnesses.size(); i++)
		if (witnesses[i]->getFamous() == true)
		{
			int num = witnesses[i]->getData().owner;
			if (b[num] == 1)
			{
				b[num] = -1;
				arr[num] = NULL;
			}
			if (b[num] == 0){
				b[num] = 1;
				arr[num] = witnesses[i];
			}
		}
	return arr;
}

void    Person::insertEvent(Event* event){
	unsigned int i;

	for (i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getRoundRecieved() != -1
			&& hashgraph[i]->getRoundRecieved() <= event->getRoundRecieved())
			break;
	while (i != hashgraph.size() &&
		(hashgraph[i]->getRoundRecieved() == -1
			|| hashgraph[i]->getRoundRecieved() == event->getRoundRecieved())
		&& hashgraph[i]->getConsensusTimestamp() <= event->getConsensusTimestamp())
		i++;

	while (i != hashgraph.size() &&
		(hashgraph[i]->getRoundRecieved() == -1
			|| hashgraph[i]->getRoundRecieved() == event->getRoundRecieved())
		&& hashgraph[i]->getConsensusTimestamp() == event->getConsensusTimestamp()
		 && hashgraph[i]->getData().owner < hashgraph[i]->getData().owner)
		 //CHANGE INDEX TO SIGNATURE YA PLEB
		i++;
	hashgraph.insert(hashgraph.begin() + i, event);
}

void Person::findOrder(){
	std::vector<Event*> w;
	std::array<Event*, N> ufw;
	unsigned int i;
	std::vector<Event*> db;
	std::vector<double> s;
	Event *tmp;

	for (unsigned int n = hashgraph.size() - 1; n < hashgraph.size(); n--){
		for (int r = hashgraph[n]->getRound(); r <= hashgraph[0]->getRound(); r++)
		{
			w = findWitnesses(r);
			std::cout << w.size() << std::endl;
			for (i = 0; i < w.size(); i++)
				if (w[i]->getFamous() == -1)
					break ;
			if (i < w.size())
				continue ;
			ufw = findUFW(w);
			int j;
			for (j = 0; j < N; j++)
				if (ufw[j] && !(ufw[j]->ancestor(hashgraph[n])))
					break;
			if (j < N)
				continue;
			hashgraph[n]->setRoundReceived(r);
			for (int j = 0; j < N; j++)
			{
				if (!ufw[j])
					continue ;
				tmp = ufw[j];
				while (tmp->getSelfParent() && tmp->getSelfParent()->ancestor(hashgraph[n])){
					tmp = tmp->getSelfParent();
				}
				s.push_back(tmp->getData().timestamp);
			}
	std::cout << s.size() <<"a9\n";
			std::sort(s.begin(),s.end());
			if (s.size() % 2)
				hashgraph[n]->setConsensusTimestamp(s[s.size() / 2]);
			else
				hashgraph[n]->setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
			break;
	std::cout << "a10\n";
		}
	}
}

void Person::gossip(Person &p){
	std::vector<data> arr;
	bool b[N] = {false};
	Event* check = getTopNode(p);
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		if (check && check->see(hashgraph[i]))
			continue;
		arr.push_back(hashgraph[i]->getData());
	}
	std::cout << "1\n";

    p.recieveGossip(*this, arr);
}

Event *Person::getTopNode(Person &target){
	Event *top = NULL;
	int t = -1;
	for (unsigned int i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getData().owner == target.index && hashgraph[i]->getData().timestamp > t)
		{
			t = hashgraph[i]->getData().timestamp;
			top = hashgraph[i];
		}
	return top;
}

void Person::createEvent(int time, Person &gossiper){
	data d;
	d.owner = index;
	d.selfP = (getTopNode(*this) ? getTopNode(*this)->getHash() : "\0");
	d.gossipP = (getTopNode(gossiper) ? getTopNode(gossiper)->getHash() : "\0");
	d.timestamp = time;
	Event *tmp = new Event(*this, d);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<data> gossip){
	unsigned int n;
	double t;
	Event *tmp;
	std::vector<Event*> nEvents;
	std::ostringstream s;
	std::cout << "d2\n";

	for (unsigned int i = 0; i < gossip.size(); i++)
	{
		s.clear();
		s << Event(*this, gossip[i]);
		for (n = 0; n < hashgraph.size(); n++)
			if (!(hashgraph[n]->getHash().compare(md5_hash(s.str()))))
				break ;
		if (n < hashgraph.size())
			continue ;
	std::cout << "c\n";
		Event *tmp = new Event(*this, gossip[i]);
		hashgraph.insert(hashgraph.begin(), tmp);
		nEvents.push_back(tmp);
	}
	std::cout << "3\n";
	createEvent(runTime, gossiper);
	nEvents.push_back(hashgraph[0]);
	std::cout << "4\n";
	for (unsigned int i = 0; i < nEvents.size(); i++)
		for (unsigned int j = i; j < nEvents.size(); j++)
			if (nEvents[j]->getData().timestamp < nEvents[i]->getData().timestamp)
			{
				tmp = nEvents[i];
				nEvents[i] = nEvents[j];
				nEvents[j] = tmp;
			}
	std::cout << "5\n";
	linkEvents(nEvents);
	std::cout << "6\n";
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->divideRounds();
	std::cout << "7\n";
	removeOddballs();
	std::cout << "8\n";
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->decideFame();
		std::cout << "9\n";
	findOrder();
		std::cout << "10\n";
}

void Person::linkEvents(std::vector<Event*> nEvents)
{
	bool shelf = false;
	bool goship = false;

	for (int i = 0; i < nEvents.size(); i++)
		if (nEvents[i]->getSelfParent() == NULL && nEvents[i]->getData().selfP != "\0")
		{
			std::string targetSelf = nEvents[i]->getData().selfP;
			std::string targetGossip = nEvents[i]->getData().gossipP;
			int c = 0;
std::cout << "targetSelf " << targetSelf << "\n";
std::cout << "targetGossip " << targetGossip << "\n";
			for (int j = 0; j < hashgraph.size(); j++)
			{
std::cout << hashgraph[j]->getData().owner << "  " << hashgraph[j]->getData().timestamp << "   " << hashgraph[j]->getHash() << "\n";
				if (hashgraph[j]->getHash() == targetSelf)
				{
					std::cout << "SELF\n";
					nEvents[i]->setSelfParent(hashgraph[j]);
					shelf = true;
					if (goship)
						break;
				}
				if (hashgraph[j]->getHash() == targetGossip)
				{
					std::cout << "GOSS\n";
					nEvents[i]->setGossiperParent(hashgraph[j]);
					goship = true;
					if (shelf)
						break;
				}
			}
		}
}

bool Person::operator==(Person &rhs){
	return index == rhs.index;
}

std::vector<Event*>    *Person::getHashgraph(){
	return &hashgraph;
}

int Person::getCurRound(){
	return currentRound;
}

void Person::incCurRound(){
	currentRound++;
}

void Person::removeOddballs(){
	for (unsigned int i = 0; i < hashgraph.size(); i++){
		if (hashgraph[i]->getRound() < currentRound - 4){
			hashgraph.erase(hashgraph.begin() + i);
			i--;
		}
	}
}
