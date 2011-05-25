////////////////////////////////// TextList.cpp ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

TextList::TextList(int cx, int cy, Font *cf) 
{
	x=cx;
	y=cy;
	f=cf;
	pos=0;
	start=0;
	elements=0;
	for (unsigned int j=0; j<5; j++)
	{
		for (unsigned int i=0; i<512; i++)
		{
			text[j][i]=(char)NULL;
		}
	}
}

void TextList::addText(char *buffer)
{
	if (elements==5)
	{
		start++;
	}
	if (start==5)
	{
		start=0;
	}
	for (int i=0; i<512; i++)
	{
		text[pos][i]=buffer[i];
	}
	pos++;
	if (elements<5)
	{
		elements++;
	}
	if (pos==5)
	{ 
		pos=0;
	} 
}

void TextList::render(SDL_Surface *screen)
{
	for (int i=0; i<5; i++)
	{
		int index=start+i;
		if (index>4) { index=index-5; }
		f->drawString(screen, 0, (i*14), text[index]);
	}
}

void TextList::action(SDL_Event event)
{
}

TextList::~TextList()
{
}