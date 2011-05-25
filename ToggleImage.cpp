////////////////////////////////// ToggleImage.cpp /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

ToggleImage::ToggleImage(int cx, int cy, const char *con, const char *coff)
{
	btoggle=false;
	x=cx;
	y=cy;
	SDL_Surface *temp = SDL_LoadBMP(con);
	if (temp==NULL) throw "Error reading file\n";
	on = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	temp = SDL_LoadBMP(coff);
	if (temp==NULL) throw "Error reading file\n";
	off = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}

void ToggleImage::action(SDL_Event event)
{
}

void ToggleImage::render(SDL_Surface *screen)
{			
	SDL_Rect pos;
	pos.x=x;
	pos.y=y;

	if (btoggle)
	{
		SDL_BlitSurface(on,NULL,screen,&pos);
	} else 
	{
		SDL_BlitSurface(off,NULL,screen,&pos);
	}
}

void ToggleImage::toggle()
{
	if (btoggle)
	{
		btoggle=false;
	} else 
	{
		btoggle=true;
	}
}

bool ToggleImage::toggled()
{
	return btoggle;
}

ToggleImage::~ToggleImage()
{
	if (on!=NULL) SDL_FreeSurface(on);
	if (off!=NULL) SDL_FreeSurface(off);
}