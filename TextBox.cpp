////////////////////////////////// TextBox.cpp /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Decims.h"

TextBox::TextBox(int cx, int cy, Font *cf)
{
	x=cx;
	y=cy;
	f=cf;
	pos=0;
	text = (char*)malloc(101);
	for (unsigned int i=0; i<101; i++)
	{
		text[i]=(char)NULL;
	}
}
void TextBox::render(SDL_Surface *screen)
{
	f->drawString(screen, 0, 84, text);
}

void TextBox::action(SDL_Event event)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
		switch(event.key.keysym.sym)
		{
			case SDLK_BACKSPACE: {	if (pos>0) {  pos--; text[pos]=-51; } break; }
			case SDLK_SPACE: {	if (pos<100) { text[pos]=' '; pos++; }break; }
			default :
			{
				if (pos<100)
				{ 
					if (event.key.keysym.sym>=97 && event.key.keysym.sym<=122)
					{
						int mod=0;
						if (event.key.keysym.mod==1 || event.key.keysym.mod==SDLK_CAPSLOCK ||event.key.keysym.mod==SDLK_LSHIFT) { mod = -32; }

						text[pos]=event.key.keysym.sym+mod; pos++; 
					}
				}
			break;
			}
		}
	}
}

const char *TextBox::getText()
{
	return text;
}

void TextBox::setText(const char* ctext)
{
	for (unsigned int i=0; i<100; i++)
	{
		if (i<strlen(ctext))
		{
			text[i]=ctext[i];
		} else {
			text[i]=(char)NULL;
		}
	}
	pos=strlen(ctext);
	return;
}

TextBox::~TextBox()
{
	free(text);
}