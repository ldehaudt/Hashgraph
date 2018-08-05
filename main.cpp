#include "Hashgraphs.hpp"

time_t start_time;
int testingNum = 1000;
SDL_Window *win;
SDL_Renderer *rend;
SDL_Event event;
bool stop = 0;

void    printHash(Person* p){
	std::cout << "Printing person # " << p->index << "\n";
	for (unsigned int i = 0; i < p->getHashgraph().size(); i++){
		std::cout << p->getHashgraph()[i]->tVal << "; " << p->getHashgraph()[i]->getData().timestamp << "\n";
	}
}

void square(Event *e)
{
	SDL_Rect rect;
	int y = 1350 - e->getData().timestamp * 20;
	int x = 100 + e->getOwner().index * 800 / (N - 1);
	if (e->getFamous() == 1)
		SDL_SetRenderDrawColor(rend, 247, 185, 0, 255);
	else if (e->getWitness() == 1)
		SDL_SetRenderDrawColor(rend, 163, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(rend, 9, 71, 124 - e->getRound() * 20, 255);
	rect.w = 10;
	rect.h = 10;
	rect.x = x - 5;
	rect.y = y - 5;
	SDL_RenderFillRect(rend, &rect);
}

void connect(Event *e, Event *p)
{
	int y = 1350 - e->getData().timestamp * 20;
	int x = 100 + e->getOwner().index * 800 / (N - 1);
	int y2 = 1350 - p->getData().timestamp * 20;
	int x2 = 100 + p->getOwner().index * 800 / (N - 1);
	SDL_RenderDrawLine(rend, x, y, x2, y2);
}

void refresh(Person *p)
{
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, NULL);
	SDL_SetRenderDrawColor(rend, 200, 200, 200, 255);
	for (int i = 0; i < N; i++)
		SDL_RenderDrawLine(rend, 100 + i * 800 / (N - 1), 0, 100 + i * 800 / (N - 1), 1400);
	for (unsigned int i = 0; i < p->getHashgraph().size(); i++)
	{
		square(p->getHashgraph()[i]);
		SDL_SetRenderDrawColor(rend, 9, 71, 124, 255);
		if (((p->getHashgraph())[i])->getSelfParent())
		{
			connect(p->getHashgraph()[i], ((p->getHashgraph())[i])->getSelfParent());
			connect(p->getHashgraph()[i], ((p->getHashgraph())[i])->getGossiperParent());
		}
	}
	SDL_RenderPresent(rend);
}

int main(){
	std::array<Person*, N> people;
	auto t_start = std::chrono::high_resolution_clock::now();
	time(&start_time);
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	win= SDL_CreateWindow("Hashgraph", 1600, 0,
		1000, 1400, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, 0);
	for (int i = 0; i < N; i++)
		people[i] = new Person(i);
	sleep(1);
	while (1)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT || event.type == SDLK_ESCAPE)
			exit (1);
		if (event.type == SDLK_KP_SPACE)
			stop = !stop;
		if (stop)
			continue ;
		int i = std::rand() % N;
		// std::cout << i << std::endl;
		int j;
		while ((j = std::rand() % N) == i)
			;
		// std::cout << j << std::endl;
		people[i]->gossip(*(people[j]));
		refresh(people[j]);
		sleep(1);
	}
}
