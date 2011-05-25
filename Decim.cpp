#include "Decims.h"

Decim::Decim(SDL_Surface *cimages[11])
{
	for (int i=0; i<11; i++)
	{
		images[i]=cimages[i];
	}
	regenerate();
}

void Decim::regenerate()
{
	value=rand() % 10 + 1;
	image=images[value-1];
}

void Decim::setValue(int val)
{
	if (val>=1 && val <=10)
	{
		value=val;
		image=images[val-1];
	}
}

Decim::~Decim()
{
	image=NULL;
	for (int i=0; i<11; i++)
	{
		images[i]=NULL;
	}
}
