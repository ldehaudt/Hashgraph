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
	Event(Person &, Event *self, Event *gossiper, double t);
	bool operator==(Event &);

	bool see(Event);
	bool seeRecursion(Event y, std::vector<Event> *);
	bool stronglySee(Event);
	void divideRounds();
	Event   *getSelfParent() const;
	Person &getOwner() const; 
	Event   *getGossiperParent() const;
	double  getTimestamp() const;
	int     getRound() const;
	bool    getWitness() const;
	double  getConsensusTimestamp() const;
	int     getRoundRecieved() const;
	char    getFamous() const;
	bool    getVote() const;

	void    setFamous(char);
	void    setVote(bool);
	void    setRoundReceived(int);
	void    setConsensusTimestamp(double);
	void	setSelfParent(Event *);
	void	setGossiperParent(Event *);

	static bool fork(Event&, Event&);        
};

#endif
