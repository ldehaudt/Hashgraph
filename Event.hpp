#ifndef EVENT_HPP
#define EVENT_HPP
#include "Person.hpp"
#include "Hashgraphs.hpp"

typedef struct s_data{
	//t_byte selfP[32];
    int selfP;
    //t_byte gossipP[32];
    int gossipP;
	int timestamp;
	int owner; //curently owner ID
}               data;    

class Person;

class Event{
private:
	Person &owner; //PROB KILL
	//PAYLOAD  MISSING
	//t_byte hash[32];
	int hash;
	data d;
	Event *selfParent;
	Event *gossiperParent;
	double   consensusTimestamp;
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
	Event(Person &p, int selfHash, int gossipHash, unsigned long t);
	bool operator==(Event &);
	bool operator<(const Event &) const;
	bool see(Event*);
	bool seeRecursion(Event *, std::vector<Event*> *);
	bool stronglySee(Event*);
	void divideRounds();
	Event   *getSelfParent() const;
	Person &getOwner() const; 
	Event   *getGossiperParent() const;
	data   getData() const;
	int     getRound() const;
	bool    getWitness() const;
	unsigned long   getConsensusTimestamp() const;
	int     getRoundRecieved() const;
	char    getFamous() const;
	bool    getVote() const;
	int		getHash() const;

	void    setFamous(char);
	void    setVote(bool);
	void    setRoundReceived(int);
	void    setConsensusTimestamp(unsigned long );
	void	setSelfParent(Event *);
	void	setGossiperParent(Event *);

//KILLLLLLLLL MMMEEEEEEEE
	int tVal; //KILL MEEEEEEEEEEEEE
//KILLLLL MMEEEEEEEEEEE


	static bool fork(Event*, Event*);        
};

#endif
