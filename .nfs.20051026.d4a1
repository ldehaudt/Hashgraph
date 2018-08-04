#include "Hashgraphs.hpp"

unsigned long startTime = clock();
int testingNum = 1000;
SDL_Window *win;
SDL_Renderer *rend;
SDL_Event event;

void    printHash(Person* p){
	std::cout << "Printing person # " << p->index << "\n";
	for (unsigned int i = 0; i < p->getHashgraph().size(); i++){
		std::cout << p->getHashgraph()[i].tVal << "; " << p->getHashgraph()[i].getTimestamp() << "\n";
	}
}

void square(Event *e)
{
	SDL_Rect rect;
	int y = 1000 - e->getTimestamp() / 1500;
	int x = 100 + e->getOwner().index * 800 / (N - 1);
	rect.w = 20;
	rect.h = 20;
	rect.x = x - 10;
	rect.y = y - 10;
	SDL_RenderFillRect(rend, &rect);
}

void connect(Event *e, Event *p)
{
	int y = 1000 - e->getTimestamp() / 1500;
	int x = 100 + e->getOwner().index * 800 / (N - 1);
	int y2 = 1000 - p->getTimestamp() / 1500;
	int x2 = 100 + p->getOwner().index * 800 / (N - 1);
	SDL_RenderDrawLine(rend, x, y, x2, y2);
}

void refresh(Person *p)
{
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, NULL);
	SDL_SetRenderDrawColor(rend, 9, 71, 124, 255);

	for (int i = 0; i < N; i++)
		SDL_RenderDrawLine(rend, 100 + i * 800 / (N - 1), 0, 100 + i * 800 / (N - 1), 1000);
	for (unsigned int i = 0; i < p->getHashgraph().size(); i++)
	{
		square(p->getHashgraph()[i]);
		if (((p->getHashgraph())[i])->getSelfParent())
		{
			std::cout << "STUFF : "<< ((p->getHashgraph())[i])->tVal << std::endl;
			std::cout << "STUFF : "<< ((p->getHashgraph())[i])->getSelfParent() << std::endl;
			std::cout << "STUFF : "<< ((p->getHashgraph())[i])->getSelfParent()->tVal << std::endl;
			connect(p->getHashgraph()[i], *(((p->getHashgraph())[i])->getSelfParent()));
			//connect(p->getHashgraph()[i], *(((p->getHashgraph())[i]).getGossiperParent()));
		}
	}
	SDL_RenderPresent(rend);
}

int main(){
	std::array<Person*, N> people;
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	win= SDL_CreateWindow("Hashgraph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1000, 1000, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, 0);
	for (int i = 0; i < N; i++)
		people[i] = new Person(i);
	SDL_PollEvent(&event);

std::cout << startTime << std::endl;
	while (event.type != SDL_QUIT)
	{
		int i;
		while ((i = std::rand() % N) != 1)
			;
		// int i = std::rand() % N;
		std::cout << i << std::endl;
		int j = 1;
		// while ((j = std::rand() % N) == i)
		// 	;
		std::cout << j << std::endl;
		people[i]->gossip(*(people[j]));
		printHash(people[j]);
		for (int i = 0; i < people[j]->getHashgraph().size(); i++)
			std::cout << "TREE 1 : " << people[j]->getHashgraph()[i] << std::endl;
		refresh(people[j]);
		sleep(1);
		SDL_PollEvent(&event);
	}
}
