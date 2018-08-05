#include "Person.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>

std::vector<Event*> Person::findWitnesses(int round){
	std::vector<Event*> witnesses;
	for (unsigned int i = 0; i < getHashgraph().size()
		&& getHashgraph()[i]->getRound() >= round - 1; i++)
		if (getHashgraph()[i]->getRound() == round
			&& getHashgraph()[i]->getWitness() == true)
			witnesses.push_back(getHashgraph()[i]);
	return witnesses;
}

void Person::decideFame(){
	int d;
	std::vector<Event*> s;
	bool v;
	int t;

	for (unsigned int x = hashgraph.size() - 1; x < hashgraph.size(); x--)
	{
		if (hashgraph[x]->getFamous() != -1)
			continue;
		for (unsigned int y = hashgraph.size() - 1; y < hashgraph.size(); y--)
			if (hashgraph[x]->getWitness() && hashgraph[y]->getWitness() &&
				hashgraph[y]->getRound() > hashgraph[x]->getRound())
			{
				d = hashgraph[y]->getRound() - hashgraph[x]->getRound();
				s = findWitnesses(hashgraph[y]->getRound() - 1);
				for (unsigned int t = 0; t < s.size(); t++)
					if (! (hashgraph[y]->stronglySee(s[t])))
						s.erase(s.begin() + t--);
					unsigned int count = 0;
					for (unsigned int i = 0; i < s.size(); i++)
						if (s[i]->getVote() == true)
							count++;
					v = (count * 2 >= s.size()) ? true: false;
					count = 0;
					for (unsigned int i = 0; i < s.size(); i++)
						if (s[i]->getVote() == v)
							count++;
					t = count;
					if (d == 1)
						hashgraph[y]->setFamous(hashgraph[y]->see(hashgraph[x]));
					else
					{
						if (d % C > 0)
						{
							if (t > 2 * N / 3)
							{
								hashgraph[x]->setFamous(v);
								hashgraph[y]->setVote(v);
								break;
							}
							else
								hashgraph[y]->setVote(v);
						}
						else
						{
							if (t > 2 * N / 3)
								hashgraph[y]->setVote(v);
							else
								hashgraph[y]->setVote(hashgraph[y]->getOwner().index % 2);
							//COIN TOSS SHOULD BE UNIFORM ?? NOT CORRECT !
						//should be : middle bit of y.signature
						}
					}
			}
	}
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
	time_t now;
	double t;

	time(&now);
	t = difftime(now, start_time);
	Event* tmp = new Event(*this, 0, 0, t);
	hashgraph.insert(hashgraph.begin(), tmp);
}

std::array<Event*, N> findUFW(std::vector<Event*> witnesses){
	std::array<Event*, N> arr;
	char b[N] = { 0 };

	for(unsigned int i = 0; i < witnesses.size(); i++){
		if (witnesses[i]->getFamous() == true)
		{
			int num = witnesses[i]->getOwner().index;                        
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
   		 && hashgraph[i]->getOwner().index < hashgraph[i]->getOwner().index)
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

	for (unsigned int n = hashgraph.size() - 1; n < hashgraph.size(); n--)
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
				if (!(ufw[j]->seeRecursion(hashgraph[n], &db)))
					break;
			if (j < N)
				continue;
			hashgraph[n]->setRoundReceived(r);
			for (int j = 0; j < N; j++)
			{
				tmp = ufw[j];
				while (tmp->getSelfParent()->seeRecursion(hashgraph[n], &db))
					tmp = tmp->getSelfParent();
				s.push_back(tmp->getData().timestamp);
			}
			std::sort(s.begin(),s.end());
			if (s.size() % 2 != 0)
				hashgraph[n]->setConsensusTimestamp(s[s.size() / 2]);
			else
				hashgraph[n]->setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
			Event *evnt = hashgraph[n];
			hashgraph.erase(hashgraph.begin() + n);
			insertEvent(evnt);
			break;
		}
}

void Person::gossip(Person &p){
	std::vector<Event> arr;
	bool b[N] = {false};
	
	Event* check = getTopNode(*this, p);
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		if (check && b[hashgraph[i]->getOwner().index] == true)
			continue;
		if (check && check->see(hashgraph[i]))
		{
			b[hashgraph[i]->getOwner().index] = true;
			continue;
		}
		arr.insert(arr.end(), *(hashgraph[i]));
	}
    p.recieveGossip(*this, arr);
std::cout << std::endl;
}

Event *Person::getTopNode(Person &p, Person &target){
	Event *top = NULL;
	int t = -1;
	for (unsigned int i = 0; i < p.hashgraph.size(); i++)
		if (p.hashgraph[i]->getOwner() == target && p.hashgraph[i]->getData().timestamp > t){
			t = p.hashgraph[i]->getData().timestamp;
			top = p.hashgraph[i];
		}
	return top;
}

void Person::createEvent(double time, Person &gossiper){
	Event *tmp = new Event(*this, Person::getTopNode(*this, *this)->getHash(),
		Person::getTopNode(*this, gossiper)->getHash(), time);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event> gossip){
	unsigned int n;
	time_t now;
	double t;
	Event *tmp;
	std::vector<Event*> nEvents;

	// for (int count = 0; count < 10; count++)
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
	time(&now);
	t = difftime(now, start_time);
	createEvent(t, gossiper);
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
	for (unsigned int i = 0; i < nEvents.size(); i++){
		nEvents[i]->divideRounds();
	}
}

void Person::linkEvents(std::vector<Event*> nEvents){
	for (int i = 0; i < nEvents.size(); i++)
	{
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
}

bool Person::operator==(Person &rhs){
	return index == rhs.index;
}

std::vector<Event*>    Person::getHashgraph(){
	return hashgraph;
}
