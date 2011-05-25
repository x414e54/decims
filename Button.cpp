
#include "Decims.h"

Button::Button(int cx, int cy, const char* ctext, Font *cf, const char* on, const char* off)
{
	SDL_Surface *temp = SDL_LoadBMP(on);
	if (temp==NULL) throw "Error reading file\n";
	onimg = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	temp = SDL_LoadBMP(off);
	if (temp==NULL) throw "Error reading file\n";
	offimg = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	x=cx;
	y=cy;
	text=(char *)malloc(strlen(ctext)+1);
	strcpy(text,ctext);
	pressed=false;
	actionready=false;
	f=cf;
}

void Button::action(SDL_Event event)
{
	switch(event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
					if (event.button.x >= x && event.button.y >= y && event.button.x <= (x + offimg->clip_rect.w) && event.button.y <= (y + offimg->clip_rect.h))
					{ 
						pressed=true;
					}
				break;
			}
		break;
		case SDL_MOUSEBUTTONUP:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
					pressed=false;
					if (event.button.x >= x && event.button.y >= y && event.button.x <= (x + onimg->clip_rect.w) && event.button.y <= (y + onimg->clip_rect.h))
					{ 
						actionready=true;
					}
				break;
			}
	}
}

void Button::render(SDL_Surface *screen)
{
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	if (pressed) {
		SDL_BlitSurface(onimg,NULL,screen,&rect);
		f->drawString(screen, x+20,y+((onimg->clip_rect.h-f->fontd.charHeight)/2), text);
	} else {
		SDL_BlitSurface(offimg,NULL,screen,&rect);
		f->drawString(screen, x+20,y+((offimg->clip_rect.h-f->fontd.charHeight)/2), text);
	}
}

bool Button::hasAction()
{
	if (actionready) { actionready=false; return true; }
	return false;
}

Button::~Button()
{
	if (onimg!=NULL) SDL_FreeSurface(onimg);
	if (offimg!=NULL) SDL_FreeSurface(offimg);
	free(text);
}

