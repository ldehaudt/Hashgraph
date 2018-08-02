#ifndef PERSON_HPP
#define PERSON_HPP

#include <ctime>
#include <list>
#include <vector>

const int N = 4; //number of members in the entire population (n > 1)
const int C = 10; // a small integer constant greater than 2, such as c = 10

class Person;

extern time_t startTime;
extern std::vector<Person*> people;

/*
For example, one event by Alice (red) records the fact that Bob performed a gossip sync in which he sent her everything he
 knew. This event is created by Alice and signed by her, and contains the hashes of two other events: her last event and Bob’s 
 last event prior to that gossip sync. The red event can also contain a payload of any transactions that Alice
 chooses to create at that moment, and perhaps a timestamp which is the time and date that Alice claims to have created it.
 */

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
        Event(Event &);
        Event & operator=(Event &);
        Event(Person &, Event *self, Event *gossiper);
        bool operator==(Event &);

        bool see(Event);
        bool seeRecursion(Event y, std::vector<Event> *);
        
        bool stronglySee(Event);

        void divideRounds();

        Event   *getSelfParent();
        Person &getOwner(); 
        Event   *getGossiperParent();
        double  getTimestamp();
        int     getRound();
        bool    getWitness();
        double  getConsensusTimestamp();
        int     getRoundRecieved();
        char    getFamous();
        void    setFamous(char);
        bool    getVote();
        void    setVote(bool);
    
        static bool fork(Event&, Event&);        
};

class Person{
    private:
        std::list<Event> hashgraph;
        void gossip(Person &);
        void createEvent(time_t time, Person &gossiper);
        static Event *getTopNode(Person &, Person &);        

    public:
        int index;
        bool operator==(Person &);
        void recieveGossip(Person &, std::vector<Event> gossip);
        std::list<Event>    getHashgraph();
        void decideFame();
        void findOrder();
        std::vector<Event> findWitnesses(int round);
};

#endif