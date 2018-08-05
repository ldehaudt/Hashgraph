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
	int owner;
	std::vector<Event*> *graph;
	//PAYLOAD  MISSING
	//t_byte hash[32];
	int hash;
	data d;
	Event *selfParent;
	Event *gossiperParent;
	int   consensusTimestamp;
	int     roundRecieved;
	int     round;
	bool    witness;
	char    famous;
	//missing signature
public:
	Event();
	~Event();
	Event(const Event &);
	Event & operator=(const Event &);
	Event(int p, int selfHash, int gossipHash, int t);
	bool operator==(Event &);
	bool operator<(const Event &) const;
	bool see(Event*);
	bool seeRecursion(Event *, std::vector<Event*> *);
	bool stronglySee(Event*);
	void divideRounds();
	Event   *getSelfParent() const;
	int getOwner() const; 
	Event   *getGossiperParent() const;
	data   getData() const;
	int     getRound() const;
	bool    getWitness() const;
	int   getConsensusTimestamp() const;
	int     getRoundRecieved() const;
	char    getFamous() const;
	int		getHash() const;
	std::vector<Event*> *getGraph() const;
	void    setFamous(char);
	void    setRoundReceived(int);
	void    setConsensusTimestamp(int );
	void	setSelfParent(Event *);
	void	setGossiperParent(Event *);
	void	decideFame();

//KILLLLLLLLL MMMEEEEEEEE
	int tVal; //KILL MEEEEEEEEEEEEE
//KILLLLL MMEEEEEEEEEEE

	static bool fork(Event*, Event*);        
};

#endif
