////////////////////////////////// DragImage.cpp ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

DragImageSet::DragImageSet()
{
	dragging=false;
	drag=NULL;
}

void DragImageSet::addDragImage(DragImage *image)
{
	imageset.push_back(image);
}

DragImage *DragImageSet::getDragImage(unsigned int index) 
{
	if (index<imageset.size()) { return imageset.at(index);}
	return NULL;
}

void DragImageSet::action(SDL_Event event)
{
	if (!dragging)
	{
		for (unsigned int i=0; i<imageset.size(); i++)
		{
			imageset.at(i)->action(event);
		}
	} else
	{
		drag->action(event);
	}
}

void DragImageSet::render(SDL_Surface *screen)
{
	for (unsigned int i=0; i<imageset.size(); i++)
	{
		if ((dragging && (imageset.at(i) != drag)) || !dragging)
		{
			imageset.at(i)->render(screen);
		}
	}
	if (dragging)
	{
		drag->render(screen);
	}
}

bool DragImageSet::drop(SDL_Event event)
{
	if (!dragging) return false;
	DragImage *onto;
	for (unsigned int i=0; i<imageset.size(); i++)
	{
		onto = imageset.at(i);
		if (onto!=drag)
		{
			if (event.button.x >= onto->x && event.button.y >= onto->y && event.button.x <= (onto->x + onto->image->clip_rect.w) && event.button.y <= (onto->y + onto->image->clip_rect.h))
			{ 
				if (!onto->moveto) {return false;}
				Decim *tmp=*drag->decim;
				*drag->decim=*onto->decim;
				*onto->decim=tmp;
				drag->x=drag->startx;
				drag->y=drag->starty;
				dragging=false;
				return true;
			}
		}
	}
	return false;
}

DragImageSet::~DragImageSet()
{
	for (unsigned int i=0; i<imageset.size(); i++)
	{
		delete imageset.at(i);
	}
	imageset;
}

DragImage::DragImage(int cx, int cy, SDL_Surface *cimage, DragImageSet *cset)
{
	set=cset;
	image=cimage;
	x=cx;
	startx=x;
	y=cy;
	starty=y;
	dragging=false;
	decim=NULL;
	movefrom=true;
	moveto=true;
}

void DragImage::action(SDL_Event event)
{
	switch(event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
					if (dragging)
					{
						if (!set->drop(event))
						{
							x=startx;
							y=starty;
						}
						dragging=false;
						set->dragging=false;
					} else {
						if (*decim!=NULL && movefrom)
						{
							if (event.button.x>=x && event.button.x <= (x+image->clip_rect.w) && event.button.y>=y && event.button.y<=(y+image->clip_rect.h))
							{
								dragging=true;
								set->dragging=true;
								set->drag=this;
							}
						}
					}
				break;
				case SDL_BUTTON_RIGHT:
				break;
			}
			break;
			case SDL_MOUSEMOTION:
				if (dragging)
				{
					x+=event.motion.xrel;
					y+=event.motion.yrel;
				}
			break;
		break;
	}
}

void DragImage::render(SDL_Surface *screen)
{
	SDL_Rect pos;
	pos.x=startx;
	pos.y=starty;
	SDL_BlitSurface(image,NULL,screen,&pos);
	if (*decim!=NULL)
	{
		pos.x=x;
		pos.y=y;
		SDL_BlitSurface((*decim)->image,NULL,screen,&pos);
	}
}

void DragImage::setDecim(Decim **cdecim)
{
	decim=cdecim;
}

DragImage::~DragImage()
{
	set=NULL;
	decim=NULL;
	image=NULL;
}

