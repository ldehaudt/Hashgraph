#ifndef EVENT_HPP
#define EVENT_HPP
#include "Person.hpp"
#include "Hashgraphs.hpp"

class Person;

class Event{
private:
	Event *selfParent;
	Person &owner;
	//PAYLOAD  MISSING
	Event *gossiperParent;
	double   consensusTimestamp;
	double   timestamp;
	int     roundRecieved;
	int     round;
	bool    witness;
	char    famous;
	bool    vote;
	//missing signature
public:
	Event();
	~Event();
	Event(const Event &);
	Event & operator=(const Event &);
	Event(Person &, Event *self, Event *gossiper, unsigned long t);
	bool operator==(Event &);

	bool see(Event);
	bool seeRecursion(Event y, std::vector<Event> *);
	bool stronglySee(Event);
	void divideRounds();
	Event   *getSelfParent() const;
	Person &getOwner() const; 
	Event   *getGossiperParent() const;
	unsigned long   getTimestamp() const;
	int     getRound() const;
	bool    getWitness() const;
	unsigned long   getConsensusTimestamp() const;
	int     getRoundRecieved() const;
	char    getFamous() const;
	bool    getVote() const;

	void    setFamous(char);
	void    setVote(bool);
	void    setRoundReceived(int);
	void    setConsensusTimestamp(unsigned long );
	void	setSelfParent(Event *);
	void	setGossiperParent(Event *);

//KILLLLLLLLL MMMEEEEEEEE
	int tVal; //KILL MEEEEEEEEEEEEE
//KILLLLL MMEEEEEEEEEEE


	static bool fork(Event&, Event&);        
};

#endif
