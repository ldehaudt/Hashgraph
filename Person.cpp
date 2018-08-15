#include "Person.hpp"

std::vector<Event*>	Person::findWitnesses(int round){
	std::vector<Event*> witnesses;
	for (unsigned int i = 0; i < getHashgraph()->size()
		&& (*getHashgraph())[i]->getRound() >= round - 1; i++)
		if ((*getHashgraph())[i]->getRound() == round
			&& (*getHashgraph())[i]->getWitness() == true)
			witnesses.push_back((*getHashgraph())[i]);
	return witnesses;
}

Person::Person(){}
Person::~Person(){
	ofs.close();
}

Person::Person(Person &rhs){
	*this = rhs;    
}

Person&	Person::operator=(Person &){
	return *this;
}

Person::Person(int ind) : index(ind) {
	currentRound = 0;
	data d;
	d.owner = index;
	d.payload = 0;
	d.target = -1;
	d.selfHash = "\0";
	d.gossipHash = "\0";
	d.timestamp = runTime;
	Event* tmp = new Event(*this, d);
	std::ostringstream filename;
	filename << "Log" << ind << std::endl;
	ofs.open(filename.str(), std::ofstream::out | std::ofstream::trunc);
	hashgraph.insert(hashgraph.begin(), tmp);
	for (int i = 0; i < N; i++)
		networth.push_back(10000);
}

std::array<Event*, N>	findUFW(std::vector<Event*> witnesses){
	std::array<Event*, N> arr;
	for (int i = 0; i < N; i++)
		arr[i] = NULL;
	char b[N] = {0};

	for(unsigned int i = 0; i < witnesses.size(); i++)
		if (witnesses[i]->getFamous() == true)
		{
			int num = witnesses[i]->getData().owner;
			if (b[num] == 1)
			{
				b[num] = -1;
				arr[num] = NULL;
			}
			if (b[num] == 0)
			{
				b[num] = 1;
				arr[num] = witnesses[i];
			}
		}
	return arr;
}

void	Person::insertEvent(Event* event){
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
		i++;
	hashgraph.insert(hashgraph.begin() + i, event);
}

void	Person::findOrder(){
	std::vector<Event*> w;
	std::array<Event*, N> ufw;
	unsigned int i;
	std::vector<Event*> db;
	Event *tmp;

	for (unsigned int n = hashgraph.size() - 1; n < hashgraph.size(); n--){
		if (hashgraph[n]->getRoundRecieved() == -1)
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
			std::vector<double> s;
			for (int j = 0; j < N; j++)
			{
				if (!ufw[j])
					continue ;
				tmp = ufw[j];
				while (tmp->getSelfParent()
					&& tmp->getSelfParent()->ancestor(hashgraph[n]))
					tmp = tmp->getSelfParent();
				s.push_back(tmp->getData().timestamp);
			}
			if (s.size() == 0)
				return ;
			hashgraph[n]->setRoundReceived(r);
			std::sort(s.begin(),s.end());
			hashgraph[n]->setConsensusTimestamp(s[s.size() / 2]);
			if (hashgraph[n]->getData().payload != 0)
			{
				networth[hashgraph[n]->getData().owner] -= hashgraph[n]->getData().payload;
				networth[hashgraph[n]->getData().target] += hashgraph[n]->getData().payload;
			}
			if (writeLog){
				ofs << "Node owner: " << hashgraph[n]->getData().owner
				<< "\tTimestamp: " << hashgraph[n]->getData().timestamp << std::endl;
				if (hashgraph[n]->getData().payload)
				{
					ofs << "\tPayload: " << hashgraph[n]->getData().payload << " to "
					<< hashgraph[n]->getData().target << std::endl;
					ofs << "\tCurrent Networth: ";
					for (int i = 0; i < N; i++)
						ofs << networth[i] << "   ";
					ofs << std::endl;
				}
				ofs << "\t" << hashgraph[n]->getData().selfHash << " -Self Parent\n"
				<< "\t" << hashgraph[n]->getData().gossipHash << " -Gossip Parent\n"
				<< "\tRound Received: " << hashgraph[n]->getRoundRecieved()
				<< "\tConsensus Time: "<< hashgraph[n]->getConsensusTimestamp()
				<< std::endl << std::endl;
			}
			break;
		}
	}
}

bool	compareEventsGreater(const Event* lhs, const Event* rhs){
	return lhs->getData().timestamp > rhs->getData().timestamp;
}

bool	compareEventsLesser(const Event* lhs, const Event* rhs){
	return lhs->getData().timestamp < rhs->getData().timestamp;
}

void	Person::gossip(Person &p){
	Event *tmp;
	std::vector<data> arr;

	std::sort(hashgraph.begin(), hashgraph.end(), compareEventsGreater);
	Event* check = getTopNode(p);
	bool b[N] = {false};
	for (unsigned int i = 0; i < hashgraph.size(); i++)
	{
		if (!b[hashgraph[i]->getData().owner]){
			if (check && check->see(hashgraph[i]))
				b[hashgraph[i]->getData().owner] = true;
			arr.push_back(hashgraph[i]->getData());
		}
	}
    p.recieveGossip(*this, arr);
}

Event	*Person::getTopNode(Person &target){
	Event *top = NULL;
	int t = -1;
	for (unsigned int i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getData().owner == target.index
			&& hashgraph[i]->getData().timestamp > t)
		{
			t = hashgraph[i]->getData().timestamp;
			top = hashgraph[i];
		}
	return top;
}

//THIS FUNCTION IS ONLY TO TEST FORKS, DO NOT USE THIS UNLESS YOU WANT TO CHEAT
Event	*Person::getForkNode(Person &target){
	Event *top = NULL;
	Event *fork = NULL;
	bool topFound = false;
	int t = -1;
	int t2 = -1;
	
	for (unsigned int i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getData().owner == target.index
			&& hashgraph[i]->getData().timestamp > t)
		{
			t = hashgraph[i]->getData().timestamp;
			top = hashgraph[i];
		}
	for (unsigned int i = 0; i < hashgraph.size(); i++)
		if (hashgraph[i]->getData().owner == target.index
			&& hashgraph[i]->getData().timestamp > t2 &&
			hashgraph[i]->getData().timestamp != t)
		{
			t2 = hashgraph[i]->getData().timestamp;
			fork = hashgraph[i];
		}
	return fork;
}

void	Person::createEvent(int time, Person &gossiper){
	data d;

	d.payload = 0;
	d.target = -1;
	if (!(std::rand() % 10))
	{
		float f = (float)(std::rand() % 100000) / 100;
		int target = std::rand() % N;
		d.payload = f;
		d.target = target;
	}
	d.owner = index;
	if (makeForks && !(std::rand() % 100) && getForkNode(*this))
	{
		d.selfHash = getForkNode(*this)->getHash();
		std::cout << RED << "Person " << index << " has created a fork!\n"; 
	}
	else
		d.selfHash = (getTopNode(*this) ? getTopNode(*this)->getHash() : "\0");
	d.gossipHash = (getTopNode(gossiper) ? getTopNode(gossiper)->getHash() : "\0");
	d.timestamp = time;
	Event *tmp = new Event(*this, d);
	hashgraph.insert(hashgraph.begin(), tmp);
}

void	Person::recieveGossip(Person &gossiper, std::vector<data> gossip){
	unsigned int n;
	double t;
	Event *tmp;
	std::vector<Event*> nEvents;
	std::string hash;

	for (unsigned int i = 0; i < gossip.size(); i++)
	{
		
		std::ostringstream s;
		s << Event(*this, gossip[i]);
		hash = md5_hash(s.str());
		for (n = 0; n < hashgraph.size(); n++){
			if (hashgraph[n]->getHash() == hash)
				break ;
		}
		if (n < hashgraph.size())
			continue ;
		Event *tmp = new Event(*this, gossip[i]);
		hashgraph.insert(hashgraph.begin(), tmp);
		nEvents.push_back(tmp);
	}
	createEvent(runTime, gossiper);
	nEvents.push_back(hashgraph[0]);
	std::sort(nEvents.begin(), nEvents.end(), compareEventsLesser);
	
	// for (unsigned int i = 0; i < nEvents.size(); i++)
	// 	for (unsigned int j = i; j < nEvents.size(); j++)
	// 		if (nEvents[j]->getData().timestamp < nEvents[i]->getData().timestamp)
	// 		{
	// 			tmp = nEvents[i];
	// 			nEvents[i] = nEvents[j];
	// 			nEvents[j] = tmp;
	// 		}
	linkEvents(nEvents);
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->divideRounds();
	removeOldBalls();
	for (unsigned int i = 0; i < nEvents.size(); i++)
		nEvents[i]->decideFame();
	findOrder();
}

void	Person::linkEvents(std::vector<Event*> nEvents)
{
	for (int i = 0; i < nEvents.size(); i++)
		if (nEvents[i]->getSelfParent() == NULL && nEvents[i]->getData().selfHash != "\0")
		{
			int c = 0;
			for (int j = 0; j < hashgraph.size(); j++)
			{
				if (hashgraph[j]->getHash() == nEvents[i]->getData().selfHash)
				{
					nEvents[i]->setSelfParent(hashgraph[j]);
					c++;
					if (c == 2)
						break;
				}
				if (hashgraph[j]->getHash() == nEvents[i]->getData().gossipHash)
				{
					nEvents[i]->setGossiperParent(hashgraph[j]);
					c++;
					if (c == 2)
						break;
				}
			}
		}
}

bool	Person::operator==(Person &rhs){
	return index == rhs.index;
}

std::vector<Event*>    *Person::getHashgraph(){
	return &hashgraph;
}

int		Person::getCurRound(){
	return currentRound;
}

void	Person::incCurRound(){
	currentRound++;
}

void	Person::removeOldBalls()
{
	for (unsigned int i = 0; i < hashgraph.size(); i++){
		if (hashgraph[i]->getRound() < currentRound - 5){
			hashgraph.erase(hashgraph.begin() + i);
			i--;
		}
	}
}
