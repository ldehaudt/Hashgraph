#ifndef EVENT_HPP
#define EVENT_HPP
#include "Person.hpp"
#include "Hashgraphs.hpp" 

class Person;

typedef struct s_data{
	std::string	selfHash;
	std::string	gossipHash;
	int			timestamp;
	int			owner;
}				data;

class Event{
private:
	std::vector<Event*> *graph;
	std::string hash;
	data	d;
	Event	*selfParent;
	Event	*gossiperParent;
	int		consensusTimestamp;
	int		roundRecieved;
	int		round;
	bool	witness;
	char	famous;
	std::string makeHash();

public:
	Event();
	~Event();
	Event(const Event &);
	Event & operator=(const Event &);
	Event(Person &, data) ;
	bool	operator==(Event &);
	bool	operator<(const Event &) const;
	bool	ancestor(Event *y);
	bool	ancestorRecursion(Event *y, bool* done, std::vector<Event*> *visted);
	bool	see(Event*);
	bool	seeRecursion(Event *, std::vector<Event*> *,
		bool *, std::vector<Event*> *visited);
	bool	stronglySee(Event*);
	void	divideRounds();
	Event	*getSelfParent() const;
	Event	*getGossiperParent() const;
	data	getData() const;
	int		getRound() const;
	bool	getWitness() const;
	int		getConsensusTimestamp() const;
	int		getRoundRecieved() const;
	char	getFamous() const;
	std::string			getHash() const;
	std::vector<Event*>	*getGraph() const;
	void	setFamous(char);
	void	setRoundReceived(int);
	void	setConsensusTimestamp(int );
	void	setSelfParent(Event *);
	void	setGossiperParent(Event *);
	void	decideFame();

	static bool fork(Event*, Event*);
};

std::ostream&	operator<<(std::ostream& os, const Event& e); 

#endif
