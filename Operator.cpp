#include "Decims.h"

Operator::Operator(int t, const char* display)
{
	type=t;
	SDL_Surface *temp = SDL_LoadBMP(display);
	image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}

Operator::~Operator()
{
	SDL_FreeSurface(image);
}
