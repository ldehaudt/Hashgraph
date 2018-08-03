#ifndef EVENT_HPP
#define EVENT_HPP

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
        Event(Person &, Event *self, Event *gossiper, double t);
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

#endif
