////////////////////////////////// Image.cpp ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

Image::Image(int cx, int cy, const char* text)
{
	SDL_Surface *temp = SDL_LoadBMP(text);
	if (temp==NULL) throw "Error reading file\n";
	image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	x=cx;
	y=cy;
}

void Image::render(SDL_Surface *screen)
{
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	SDL_BlitSurface(image,NULL,screen,&rect);
}

void Image::action(SDL_Event event)
{
}

Image::~Image()
{
	if (image!=NULL) SDL_FreeSurface(image);
}

