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
	//DOES NOTHING, I DONT THIKN WE NEED THIS
}

Person::Person(int ind) : index(ind) {
	Event* tmp = new Event(index, 0, 0, runTime);
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
			int num = witnesses[i]->getOwner();                        
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
   		 && hashgraph[i]->getOwner() < hashgraph[i]->getOwner())
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
			std::sort(s.begin(),s.end());
			if (s.size() % 2 != 0)
				hashgraph[n]->setConsensusTimestamp(s[s.size() / 2]);
			else
				hashgraph[n]->setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
			std::cout << "CONSENSUS TIMESTAMP FOUND : "<< hashgraph[n]->getConsensusTimestamp() << "\n";
			Event *evnt = hashgraph[n];
			hashgraph.erase(hashgraph.begin() + n);
			insertEvent(evnt);
			break;
		}
	}
}

void Person::gossip(Person &p){
	std::vector<Event> arr;
	bool b[N] = {false};

	Event* check = getTopNode(p);
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		if (check && check->see(hashgraph[i]))
			continue;
		arr.insert(arr.end(), *(hashgraph[i]));
	}
    p.recieveGossip(*this, arr);
}

Event *Person::getTopNode(Person &target){
	Event *top = NULL;
	int t = -1;
	for (unsigned int i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getOwner() == target.index && hashgraph[i]->getData().timestamp > t)
		{
			t = hashgraph[i]->getData().timestamp;
			top = hashgraph[i];
		}
	return top;
}

void Person::createEvent(double time, Person &gossiper){
	Event *tmp = new Event(index, getTopNode(*this)->getHash(),
		getTopNode(gossiper)->getHash(), time);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event> gossip){
	unsigned int n;
	double t;
	Event *tmp;
	std::vector<Event*> nEvents;

	for (unsigned int i = 0; i < gossip.size(); i++)
	{
		for (n = 0; n < hashgraph.size(); n++)
			if (*hashgraph[n] == gossip[i])
				break ;
		if (n < hashgraph.size())
			continue ;
		Event *tmp = new Event(gossip[i]);
		hashgraph.insert(hashgraph.begin(), tmp);
		nEvents.push_back(tmp);
	}
	createEvent(runTime, gossiper);
	nEvents.push_back(hashgraph[0]);
	//MIGHT NOT BE NEEDED, CHECK FOR USEFULLNESS 
	// for (unsigned int i = 0; i < nEvents.size(); i++)
	// 	for (unsigned int j = i; j < nEvents.size(); j++)
	// 		if (nEvents[j]->getData().timestamp < nEvents[i]->getData().timestamp)
	// 		{
	// 			tmp = nEvents[i];
	// 			nEvents[i] = nEvents[j];
	// 			nEvents[j] = tmp;
	// 		}
	//END KILL
	linkEvents(nEvents);
	for (unsigned int i = 0; i < nEvents.size(); i++)
	{
		nEvents[i]->divideRounds();
	}
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->decideFame();
	findOrder();
}

void Person::linkEvents(std::vector<Event*> nEvents){

	for (int i = 0; i < nEvents.size(); i++)
		if (nEvents[i]->getSelfParent() == NULL && nEvents[i]->getData().selfP != 0)
		{
			int targetSelf = nEvents[i]->getData().selfP;
			int targetGossip = nEvents[i]->getData().gossipP;
			int c = 0;
			for (int j = 0; j < hashgraph.size(); j++)
			{
				if (hashgraph[j]->getHash() == targetSelf)
				{
					nEvents[i]->setSelfParent(hashgraph[j]);
					c++;
					if (c == 2)
						break;
				}
				if (hashgraph[j]->getHash() == targetGossip)
				{
					nEvents[i]->setGossiperParent(hashgraph[j]);
					c++;
					if (c == 2)
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
