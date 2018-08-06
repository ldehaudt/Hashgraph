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
<<<<<<< HEAD
	data d;
	d.owner = index;
	d.selfP = 0;
	d.gossipP = 0;
	d.timestamp = runTime;
	d.tVal = ++testingNum;
	Event* tmp = new Event(*this, d);
=======
	Event* tmp = new Event(index, 0, 0, runTime);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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
<<<<<<< HEAD
			int num = witnesses[i]->getData().owner;
=======
			int num = witnesses[i]->getOwner();                        
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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
<<<<<<< HEAD
		 && hashgraph[i]->getData().owner < hashgraph[i]->getData().owner)
		 //CHANGE INDEX TO SIGNATURE YA PLEB
=======
   		 && hashgraph[i]->getOwner() < hashgraph[i]->getOwner())
   		 //CHANGE INDEX TO SIGNATURE YA PLEB
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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
std::cout << "A\n";
			for (i = 0; i < w.size(); i++)
				if (w[i]->getFamous() == -1)
					break ;
std::cout << "B\n";
			if (i < w.size())
				continue ;
			ufw = findUFW(w);
std::cout << "C\n";
			int j;
			for (j = 0; j < N; j++)
				if (ufw[j] && !(ufw[j]->ancestor(hashgraph[n])))
					break;
			if (j < N)
				continue;
std::cout << "D\n";
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
std::cout << "E\n";
			std::sort(s.begin(),s.end());
std::cout << s.size() << std::endl;
			if (s.size() % 2)
				hashgraph[n]->setConsensusTimestamp(s[s.size() / 2]);
			else
				hashgraph[n]->setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
<<<<<<< HEAD
std::cout << "G\n";
=======
			std::cout << "CONSENSUS TIMESTAMP FOUND : "<< hashgraph[n]->getConsensusTimestamp() << "\n";
			Event *evnt = hashgraph[n];
			hashgraph.erase(hashgraph.begin() + n);
			insertEvent(evnt);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
			break;
		}
	}
}

void Person::gossip(Person &p){
<<<<<<< HEAD
	std::vector<data> arr;
	bool b[N] = {false};
std::cout << "dasda\n";
=======
	std::vector<Event> arr;
	bool b[N] = {false};

>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	Event* check = getTopNode(p);
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		if (check && check->see(hashgraph[i]))
			continue;
<<<<<<< HEAD
		arr.push_back(hashgraph[i]->getData());
	}
std::cout << "1\n";
=======
		arr.insert(arr.end(), *(hashgraph[i]));
	}
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
    p.recieveGossip(*this, arr);
}

Event *Person::getTopNode(Person &target){
	Event *top = NULL;
	int t = -1;
	for (unsigned int i = 0; i < hashgraph.size(); i++)
<<<<<<< HEAD
		if (hashgraph[i]->getData().owner == target.index && hashgraph[i]->getData().timestamp > t)
=======
		if (hashgraph[i]->getOwner() == target.index && hashgraph[i]->getData().timestamp > t)
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
		{
			t = hashgraph[i]->getData().timestamp;
			top = hashgraph[i];
		}
	return top;
}

void Person::createEvent(double time, Person &gossiper){
<<<<<<< HEAD
	data d;
	d.owner = index;
	d.selfP = (getTopNode(*this) ? getTopNode(*this)->getHash() : 0);
	d.gossipP = (getTopNode(gossiper) ? getTopNode(gossiper)->getHash() : 0);
	d.timestamp = time;
	d.tVal = ++testingNum;
	Event *tmp = new Event(*this, d);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<data> gossip){
=======
	Event *tmp = new Event(index, getTopNode(*this)->getHash(),
		getTopNode(gossiper)->getHash(), time);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event> gossip){
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	unsigned int n;
	double t;
	Event *tmp;
	std::vector<Event*> nEvents;

<<<<<<< HEAD
std::cout << "2\n";
=======
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	for (unsigned int i = 0; i < gossip.size(); i++)
	{
		for (n = 0; n < hashgraph.size(); n++)
			if (hashgraph[n]->getHash() == gossip[i].tVal) // replace with hash(gossip[i])
				break ;
		if (n < hashgraph.size())
			continue ;
<<<<<<< HEAD
		Event *tmp = new Event(*this, gossip[i]);
		hashgraph.insert(hashgraph.begin(), tmp);
		nEvents.push_back(tmp);
	}
std::cout << "3\n";
	createEvent(runTime, gossiper);
	nEvents.push_back(hashgraph[0]);
	for (unsigned int i = 0; i < nEvents.size(); i++)
        for (unsigned int j = i; j < nEvents.size(); j++)
            if (nEvents[j]->getData().timestamp < nEvents[i]->getData().timestamp)
            {
                tmp = nEvents[i];
                nEvents[i] = nEvents[j];
                nEvents[j] = tmp;
            }
	linkEvents(nEvents);
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->divideRounds();
std::cout << "6\n";
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->decideFame();
std::cout << "7\n";
	findOrder();
std::cout << "8\n";
=======
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
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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
<<<<<<< HEAD
					std::cout << "self !!!!!!!!!!!!!!!!!\n";
=======
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
					if (c == 2)
						break;
				}
				if (hashgraph[j]->getHash() == targetGossip)
				{
					nEvents[i]->setGossiperParent(hashgraph[j]);
					c++;
<<<<<<< HEAD
					std::cout << "gossip !!!!!!!!!!!!!!!!!\n";
=======
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
					if (c == 2)
						break;
				}
			}
			if (c == 1)
				std::cout << "WARNING !!!!!!!!!!!!!!!!!\n";
		}
}

bool Person::operator==(Person &rhs){
	return index == rhs.index;
}

std::vector<Event*>    *Person::getHashgraph(){
	return &hashgraph;
}
