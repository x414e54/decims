////////////////////////////////// ComboBox.cpp ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

ComboBox::ComboBox(int cx, int cy)
{
	x=cx;
	y=cy;
	open=false;
}

void ComboBox::addOption(Operator *op)
{
	options.push_back(op);
}

void ComboBox::action(SDL_Event event)
{
	switch(event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
					if (open) {
						int w=0;
						int h=0;
						for (unsigned int i=0; i<options.size(); i++) {
							h+=options.at(i)->image->clip_rect.h;
							if (event.button.x>=x && event.button.x <= (x+options.at(i)->image->clip_rect.w) && event.button.y>=(y-h) && event.button.y<=((y-h)+options.at(i)->image->clip_rect.h)) {
								*selected=options.at(i);
								open=false;
								break;
							}
						}
					} else {
						if (event.button.x>=x && event.button.x <= (x+(*selected)->image->clip_rect.w) && event.button.y>=y && event.button.y<=(y+(*selected)->image->clip_rect.h)) {
							open=true;
						}
					}
				break;
			}
			break;
		break;
	}
}
void ComboBox::render(SDL_Surface *screen)
{
	SDL_Rect pos;
	pos.x=x;
	pos.y=y;
	SDL_BlitSurface((*selected)->image,NULL,screen,&pos);
	if (open) {
		int w=0;
		int h=0;
		for (unsigned int i=0; i<options.size(); i++) {
			h+=options.at(i)->image->clip_rect.h;
			pos.x=x;
			pos.y=y-h;
			SDL_BlitSurface(options.at(i)->image,NULL,screen,&pos);
		}
	}
}

void ComboBox::setSelected(Operator **cselected)
{
	selected=cselected;
}

ComboBox::~ComboBox()
{
}

