////////////////////////////////// Static.cpp //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

Static::Static(int cx, int cy, const char* ctext, Font *cf)
{
	f=cf;
	x=cx;
	y=cy;
	text=(char *)malloc(strlen(ctext)+1);
	strcpy(text,ctext);
}

void Static::render(SDL_Surface *screen)
{
	f->drawString(screen, x,y, text);
}

void Static::action(SDL_Event event)
{
}

void Static::setText(const char *ctext) {
	text=(char *)realloc(text,strlen(ctext)+1);
	strcpy(text,ctext);
}

Static::~Static()
{
	free(text);
	text=NULL;
}

