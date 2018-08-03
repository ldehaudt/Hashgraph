#include "Person.hpp"
#include <list>
#include <vector>
#include <array>
#include <algorithm>

std::vector<Event> Person::findWitnesses(int round){ //probably need player whose graph to look into as param 
    std::list<Event>::iterator iter;
    std::vector<Event> witnesses;
    for (iter = getHashgraph().begin(); iter->getRound() >= round - 1; iter++){
        if (iter->getRound() == round && iter->getWitness() == true){
            witnesses.push_back(*iter);
        }
    }
    return witnesses;
 }

void Person::decideFame(){ //probably need player whose graph to look into as param
    std::list<Event>::reverse_iterator x;
    std::list<Event>::reverse_iterator y;
    int d;
    std::vector<Event> s;
    bool v;
    int t;

    for (x = hashgraph.rbegin(); x != hashgraph.rend(); x++){ // p is player (PREV COMMENT)
        if (x->getFamous() != -1)
            continue;
        for (y = hashgraph.rbegin(); y != hashgraph.rend(); y++){
            if (x->getWitness() && y->getWitness() && y->getRound() > x->getRound()){
                d = y->getRound() - x->getRound();
                s = findWitnesses(y->getRound() - 1);
                std::vector<Event>::iterator tmp;
                for (tmp = s.begin(); tmp < s.end(); tmp++)
                    if (!(*y).stronglySee(*tmp)) {
                        s.erase(tmp);
                        tmp--;
                    }
                
                int count = 0;
                for (int i = 0; i < static_cast<int>(s.size()); i++){
                    if (s[i].getVote() == true)
                        count++;
                }
                v = (count * 2 >= static_cast<int>(s.size())) ? true: false;
                count = 0;
                for (int i = 0; i < static_cast<int>(s.size()); i++){
                    if (s[i].getVote() == v)
                        count++;
                }
                t = count;
                if (d == 1){
                    y->setFamous(y->see(*x));
                }
                else{
                    if (d % C > 0){
                        if (t > 2 * N / 3){
                            x->setFamous(v);
                            y->setVote(v);
                            break;
                        }
                        else{
                            y->setVote(v);
                        }
                    }
                    else{
                        if (t > 2 * N / 3){
                            y->setVote(v);
                        }else{
                            y->setVote(y->getOwner().index % 2);//COIN TOSS SHOULD BE UNIFORM ?? NOT CORRECT !
                        }           //should be : middle bit of y.signature
                    }
                }
            }
        }
    }
}

Person::Person(){}
        
Person::~Person(){}
        
Person::Person(Person &rhs){
    *this = rhs;    
}

Person & Person::operator=(Person &){
    return *this;
    //DOES NOTHING, I DONT THIKN WE NEED THIS
}

std::array<Event*, N> findUFW(std::vector<Event> witnesses){
    std::array<Event*, N> arr;
    char b[N] = { 0 };

    for(int i = 0; i < static_cast<int>(witnesses.size()); i++){
        if (witnesses[i].getFamous() == true)
        {
            int num = witnesses[i].getOwner().index;                        
            if (b[num] == 1)
            {
                b[num] = -1;
                arr[num] = NULL;
            }
            if (b[num] == 0){
                b[num] = 1;
                arr[num] = &(witnesses[i]);
            }
        }
    }
    return arr;
}

void    Person::insertEvent(Event event){
    std::list<Event>::iterator iter;

    for (iter = getHashgraph().begin(); iter != getHashgraph().end(); iter++){
        if (iter->getRoundRecieved() != -1 && iter->getRoundRecieved() <= event.getRoundRecieved()){
            break;
        }
    }
    while (iter != getHashgraph().end() &&
    (iter->getRoundRecieved() == -1 || iter->getRoundRecieved() == event.getRoundRecieved())
    && iter->getConsensusTimestamp() <= event.getConsensusTimestamp())
        iter++;
    
    while (iter != getHashgraph().end() &&
    (iter->getRoundRecieved() == -1 || iter->getRoundRecieved() == event.getRoundRecieved())
    && iter->getConsensusTimestamp() == event.getConsensusTimestamp()
    && iter->getOwner().index < iter->getOwner().index) //CHANGE INDEX TO SIGNATURE YA PLEB
        iter++;

    hashgraph.insert(iter, event);
    
}

void Person::findOrder(){
    std::list<Event>::reverse_iterator iter;
    std::vector<Event> w;
    std::array<Event*, N> ufw;
    int i;
    std::vector<Event> db;
    

    for (iter = hashgraph.rbegin(); iter != hashgraph.rend(); iter--)
    {
        for (int r = iter->getRound(); r <= hashgraph.begin()->getRound(); r++) {
            w = findWitnesses(r);
            for (i = 0; i < static_cast<int>(w.size()); i++)
                if (w[i].getFamous() == -1)
                    break ;
            if (i < static_cast<int>(w.size()))
                continue ;
            ufw = findUFW(w);
            int j;
            for (j = 0; j < N; j++)
            {
                if (!(ufw[j]->seeRecursion(*iter, &db))){
                    break;
                }
            }
            if (j < N)
                continue;
            iter->setRoundReceived(r);
            std::vector<double> s;
            Event *tmp;
            for (int j = 0; j < N; j++){
                tmp = ufw[j];
                while (tmp->getSelfParent()->seeRecursion(*iter, &db))
                    tmp = tmp->getSelfParent();
                s.push_back(tmp->getTimestamp());
            }
            std::sort(s.begin(),s.end());
            if (s.size() % 2 != 0){
                iter->setConsensusTimestamp(s[s.size() / 2]);
            }else{
                iter->setConsensusTimestamp((s[s.size() / 2 - 1] + s[s.size() / 2]) / 2);
            }
            Event evnt = *iter;
            hashgraph.erase(--(iter.base()));
            insertEvent(evnt);
            break;
        }
    }
}

void Person::gossip(Person &p){
    std::vector<Event> arr;
    std::list<Event>::iterator iter;
    bool b[N] = {false};
    Event* check = getTopNode(*this, p);

    for (iter = hashgraph.begin(); iter != hashgraph.end(); iter++){
        if (b[iter->getOwner().index] == true)
            continue;
        if (check->see(*iter)){
            b[iter->getOwner().index] = true;
            continue;
        }
        arr.push_back(*iter);
    }
    p.recieveGossip(*this, arr);// FIND MISSING STUFF BETWEEN BOTH PEOPLE
}

Event *Person::getTopNode(Person &p, Person &target){
    std::list<Event>::iterator iter;

    for (iter = p.hashgraph.begin(); iter != p.hashgraph.end(); iter++){
        if (iter->getOwner() == target){
            return &(*iter);
        }
    }
    return NULL;
}

void Person::createEvent(time_t time, Person &gossiper){
    Event tmp(*this, Person::getTopNode(*this, *this), Person::getTopNode(*this, gossiper), time);
    hashgraph.push_front(tmp);
}

void Person::recieveGossip(Person &gossiper, std::vector<Event> gossip){
    std::list<Event>::iterator iter;
    std::list<Event>::iterator tmp;
    bool self;
    bool gos;

    for (int i = 0; i < static_cast<int>(gossip.size()); i++){
        self = false;
        gos = false;
        for (iter = hashgraph.begin(); iter != hashgraph.end(); iter++){
            if (*iter == gossip[i])
                break;
            if (*iter == *(gossip[i].getSelfParent())) {
                gossip[i].setSelfParent(&(*iter));
                self = true;
            }
            if (*iter == *(gossip[i].getGossiperParent())) {
                gossip[i].setGossiperParent(&(*iter));
                gos = true;
            }
            if (self && gos)
            {
                gossip[i].divideRounds();
                for (tmp = hashgraph.begin(); tmp != hashgraph.end(); tmp++){
                    if (tmp->getRound() <= gossip[i].getRound())
                        break;
                }
                hashgraph.insert(tmp, gossip[i]);
                break;
            }
        }
    }
    createEvent(time(0) - startTime, gossiper);    
}

bool Person::operator==(Person &rhs){
    return index == rhs.index;
}

std::list<Event>    Person::getHashgraph(){
    return hashgraph;
}
