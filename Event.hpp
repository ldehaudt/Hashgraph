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

typedef struct s_data{
	//t_byte selfP[32];
    int selfP;
    //t_byte gossipP[32];
    int gossipP;
	int timestamp;
	int owner;
	int tVal; //KILLLLLLL MEMEMEMEEMM!!!!!!!
}               data;

class Event{
private:
<<<<<<< HEAD
=======
	int owner;
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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
<<<<<<< HEAD
	Event(Person &, data) ;
=======
	Event(int p, int selfHash, int gossipHash, int t);
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
	bool operator==(Event &);
	bool operator<(const Event &) const;
	bool ancestor(Event *y);
	bool ancestorRecursion(Event *y, bool* done, std::vector<Event*> *visted);
	bool see(Event*);
	bool  seeRecursion(Event *, std::vector<Event*> *, bool *, std::vector<Event*> *visited);
	bool stronglySee(Event*);
	void divideRounds();
	Event   *getSelfParent() const;
<<<<<<< HEAD
=======
	int getOwner() const; 
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
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

<<<<<<< HEAD
	static bool fork(Event*, Event*);
=======
	int tVal; //KILL MEEEEEEEEEEEEE

	static bool fork(Event*, Event*);        
>>>>>>> 8c4ba2c8a04e696a707c97ffc0a711356b2069af
};

#endif
