#ifndef PERSON_HPP
#define PERSON_HPP

#include <ctime>
#include <list>
#include <vector>
#include "Event.hpp"
#include "Hashgraphs.hpp"

class Person;
class Event;

typedef struct s_data data;

/*
For example, one event by Alice (red) records the fact that Bob performed a gossip sync in which he sent her everything he
 knew. This event is created by Alice and signed by her, and contains the hashes of two other events: her last event and Bob’s 
 last event prior to that gossip sync. The red event can also contain a payload of any transactions that Alice
 chooses to create at that moment, and perhaps a timestamp which is the time and date that Alice claims to have created it.
 */

class Person{
private:
	std::vector<Event*> hashgraph;
	void createEvent(double time, Person &gossiper);
<<<<<<< HEAD
	Event *getTopNode(Person &);
	int currentRound;
=======
	Event *getTopNode(Person &);        
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af

public:
	Person();
	~Person();
	Person(Person &);
	Person & operator=(Person &);

	Person(int);
	int index;
	bool operator==(Person &);
<<<<<<< HEAD
	void recieveGossip(Person &, std::vector<data> gossip);
=======
	void recieveGossip(Person &, std::vector<Event> gossip);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	std::vector<Event*>    *getHashgraph();
	void findOrder();
	void linkEvents(std::vector<Event*>);
	void insertEvent(Event* event);
	std::vector<Event*> findWitnesses(int round);
	void gossip(Person &);
};

#endif