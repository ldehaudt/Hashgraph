#ifndef PERSON_HPP
#define PERSON_HPP

#include "Event.hpp"
#include "Hashgraphs.hpp"

class Person;
class Event;

typedef struct s_data data;

class Person{
private:
	std::vector<Event*> hashgraph;
	void	createEvent(int time, Person &gossiper);
	Event	*getTopNode(Person &);
	int		currentRound;
	std::ofstream ofs;
public:
	Person();
	~Person();
	Person(Person &);
	Person&	operator=(Person &);
	std::vector<float>	networth;

	Person(int);
	int	index;
	bool	operator==(Person &);
	void	recieveGossip(Person &, std::vector<data> gossip);
	std::vector<Event*>	*getHashgraph();
	void	findOrder();
	void	linkEvents(std::vector<Event*>);
	void	insertEvent(Event* event);
	std::vector<Event*>	findWitnesses(int round);
	void	gossip(Person &);
	int		getCurRound();
	void	incCurRound();
	void	removeOldBalls();
};

#endif
