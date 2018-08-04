#include "Person.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>

std::vector<Event> Person::findWitnesses(int round){
//probably need player whose graph to look into as param 
	// std::list<Event>::iterator iter;
	std::vector<Event> witnesses;
	for (unsigned int i = 0; getHashgraph()[i].getRound() >= round - 1; i++){
		if (getHashgraph()[i].getRound() == round && getHashgraph()[i].getWitness() == true){
			witnesses.push_back(getHashgraph()[i]);
		}
	}
	return witnesses;
}

void Person::decideFame(){
//probably need player whose graph to look into as param
	std::list<Event>::reverse_iterator y;
	int d;
	std::vector<Event> s;
	bool v;
	int t;

	for (unsigned int x = hashgraph.size() - 1; x < hashgraph.size(); x--)
	{
    // p is player (PREV COMMENT)
		if (hashgraph[x].getFamous() != -1)
			continue;
		for (unsigned int y = hashgraph.size() - 1; y < hashgraph.size(); y--)
		{
			if (hashgraph[x].getWitness() && hashgraph[y].getWitness() &&
				hashgraph[y].getRound() > hashgraph[x].getRound())
			{
				d = hashgraph[y].getRound() - hashgraph[x].getRound();
				s = findWitnesses(hashgraph[y].getRound() - 1);
				for (unsigned int t = 0; t < s.size(); t++)
					if (!(hashgraph[y]).stronglySee(s[t]))
					{
						s.erase(s.begin() + t);
						t--;
					}
					unsigned int count = 0;
					for (unsigned int i = 0; i < s.size(); i++)
						if (s[i].getVote() == true)
							count++;
					v = (count * 2 >= s.size()) ? true: false;
					count = 0;
					for (unsigned int i = 0; i < s.size(); i++)
						if (s[i].getVote() == v)
							count++;
					t = count;
					if (d == 1)
						hashgraph[y].setFamous(hashgraph[y].see(hashgraph[x]));
					else
					{
						if (d % C > 0)
						{
							if (t > 2 * N / 3)
							{
								hashgraph[x].setFamous(v);
								hashgraph[y].setVote(v);
								break;
							}
							else
								hashgraph[y].setVote(v);
						}
						else
						{
							if (t > 2 * N / 3)
								hashgraph[y].setVote(v);
							else
								hashgraph[y].setVote(hashgraph[y].getOwner().index % 2);
							//COIN TOSS SHOULD BE UNIFORM ?? NOT CORRECT !
						//should be : middle bit of y.signature
						}
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
	Event tmp(*this, NULL, NULL, clock() - startTime);
	hashgraph.insert(hashgraph.begin(), tmp);
}

std::array<Event*, N> findUFW(std::vector<Event> witnesses){
	std::array<Event*, N> arr;
	char b[N] = { 0 };

	for(int i = 0; i < static_cast<int>(witnesses.size()); i++){
		if (witnesses[i].getFamous() == true)
		{
			int num = witnesses[i].getOwner().index;                        
			if (b[num] == 1)
			{
				b[num] = -1;
				arr[num] = NULL;
			}
			if (b[num] == 0){
				b[num] = 1;
				arr[num] = &(witnesses[i]);
			}
		}
	}
	return arr;
}

void    Person::insertEvent(Event event){
	unsigned int i;

	for (i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i].getRoundRecieved() != -1 && hashgraph[i].getRoundRecieved() <= event.getRoundRecieved())
			break;
	while (i != hashgraph.size() &&
		(hashgraph[i].getRoundRecieved() == -1 || hashgraph[i].getRoundRecieved() == event.getRoundRecieved())
		&& hashgraph[i].getConsensusTimestamp() <= event.getConsensusTimestamp())
		i++;

	while (i != hashgraph.size() &&
		(hashgraph[i].getRoundRecieved() == -1 || hashgraph[i].getRoundRecieved() == event.getRoundRecieved())
		&& hashgraph[i].getConsensusTimestamp() == event.getConsensusTimestamp()
   		 && hashgraph[i].getOwner().index < hashgraph[i].getOwner().index) //CHANGE INDEX TO SIGNATURE YA PLEB
		i++;
	hashgraph.insert(hashgraph.begin() + i, event);
}

void Person::findOrder(){
	std::vector<Event> w;
	std::array<Event*, N> ufw;
	unsigned int i;
	std::vector<Event> db;
	std::vector<double> s;
	Event *tmp;

	for (unsigned int n = hashgraph.size() - 1; n < hashgraph.size(); n--)
	{
		for (int r = hashgraph[n].getRound(); r <= hashgraph.begin()->getRound(); r++)
		{
			w = findWitnesses(r);
			for (i = 0; i < w.size(); i++)
				if (w[i].getFamous() == -1)
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
			hashgraph[n].setRoundReceived(r);
			for (int j = 0; j < N; j++)
			{
				tmp = ufw[j];
				while (tmp->getSelfParent()->seeRecursion(hashgraph[n], &db))
					tmp = tmp->getSelfParent();
				s.push_back(tmp->getTimestamp());
			}
			std::sort(s.begin(),s.end());
			if (s.size() % 2 != 0)
				hashgraph[n].setConsensusTimestamp(s[s.size() / 2]);
			else
				hashgraph[n].setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
			Event evnt = hashgraph[n];
			hashgraph.erase(hashgraph.begin() + n);
			insertEvent(evnt);
			break;
		}
	}
}

void Person::gossip(Person &p){
	std::cout << "1\n";
	std::vector<Event> arr;
	// std::list<Event>::iterator iter;
	bool b[N] = {false};
	std::cout << "A\n";
	
	Event* check = getTopNode(*this, p);
	std::cout << "B\n";
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		std::cout << "C\n";
		if (check && b[hashgraph[i].getOwner().index] == true)
			continue;
		std::cout << "C2\n";
		if (check && check->see(hashgraph[i]))
		{
			b[hashgraph[i].getOwner().index] = true;
			continue;
		}
		std::cout << "D\n";
		arr.push_back(hashgraph[i]);
	}
	std::cout << "2\n";
	
    p.recieveGossip(*this, arr);
}

Event *Person::getTopNode(Person &p, Person &target){
	for (unsigned int i = 0; i < p.hashgraph.size(); i++)
		if (p.hashgraph[i].getOwner() == target)
			return &(p.hashgraph[i]);
	return NULL;
}

void Person::createEvent(double time, Person &gossiper){
	std::cout << "5\n";
	
	Event tmp(*this, Person::getTopNode(*this, *this), Person::getTopNode(*this, gossiper), time);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event> gossip){
	bool self;
	bool gos;
	unsigned int j;
	unsigned int n;
	std::cout << "aa3\n";

	for (unsigned int i = 0; i < gossip.size(); i++)
	{
		std::cout << "a\n";
		
		for (n = 0; n < hashgraph.size(); n++)
		{
			if (hashgraph[n] == gossip[i])
				break ;
		}
		std::cout << "b\n";
		
		if (n < hashgraph.size())
			continue ;
		self = false;
		gos = false;
		std::cout << "c\n";
		
		for (unsigned int k = 0; k < hashgraph.size(); i++)
		{
		std::cout << "d\n";
			
			if (gossip[i].getSelfParent() == NULL){
				gossip[i].divideRounds();
				for (j = 0; j < hashgraph.size(); j++)
					if (hashgraph[j].getRound() <= gossip[i].getRound())
						break;
				hashgraph.insert(hashgraph.begin() + j, gossip[i]);
				break;
			}
		std::cout << (gossip[i].getSelfParent())->tVal << "e\n";
		std::cout << gossip[i].getSelfParent() << "e\n";
		std::cout << hashgraph[k].tVal << "e\n";
			if (hashgraph[k] == *(gossip[i].getSelfParent()))
			{
				std::cout << "e2\n";
				gossip[i].setSelfParent(&hashgraph[k]);
				self = true;
			}
		std::cout << "f\n";
			if (hashgraph[k] == *(gossip[i].getGossiperParent()))
			{
		std::cout << "f2\n";
				gossip[i].setGossiperParent(&hashgraph[k]);
				gos = true;
			}
		std::cout << "g\n";
			if ((self && gos) || !gossip[i].getSelfParent())
			{
				gossip[i].divideRounds();
				for (j = 0; j < hashgraph.size(); j++)
					if (hashgraph[j].getRound() <= gossip[i].getRound())
						break;
				hashgraph.insert(hashgraph.begin() + j, gossip[i]);
				break;
			}
		}
	}
	std::cout << "4\n";
	
	createEvent(clock() - startTime, gossiper);    
}

bool Person::operator==(Person &rhs){
	return index == rhs.index;
}

std::vector<Event>    Person::getHashgraph(){
	return hashgraph;
}
