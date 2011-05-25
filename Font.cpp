#include "Decims.h"

Font::Font(const char *fontfile, int r, int g, int b)
{ 
	std::ifstream f(fontfile);
	unsigned char *unconvertedData;    
	SDL_Surface *temp;
	
	if (f.is_open())
	{
		// Fill File Header
		f.read((char *)&fontd.fontSig,2);
		f.read((char *)&fontd.fontFileWidth,4);
		f.read((char *)&fontd.fontFileHeight,4);
		f.read((char *)&fontd.charHeight,4);
		if (fontd.fontSig !=0xAABF) { f.close(); return;}

		// Fill unconverted bit data
		size_t padding=0;
		if (fontd.fontFileWidth%4!=0) { padding=4-(fontd.fontFileWidth%4); }
		size_t size =(fontd.fontFileHeight*(fontd.fontFileWidth+padding))/4;
		unconvertedData = new unsigned char[size];
	
		f.read((char *)unconvertedData,size);

		// Fill Widths
		size = 256;
		fontd.charWidths = new unsigned char[size];
		
		f.read((char *)fontd.charWidths,size);		
		f.close();
    
		// Convert the unconverted data into a normal BMP style usable by SDL
		unsigned int bit1=0x80;
		unsigned int bit2=0x40;
		unsigned int bit3=0x20;
		unsigned int bit4=0x10;
		unsigned int bit5=0x8;
		unsigned int bit6=0x4;
		unsigned int bit7=0x2;
		unsigned int bit8=0x1;

		size_t dsize =4*fontd.fontFileHeight*fontd.fontFileWidth;
		fontd.data = new unsigned char[dsize];
		unsigned int i=0;
		unsigned int bit=0;
		for (unsigned int h=0; h<fontd.fontFileHeight; h++)
		{
			for (unsigned int w=0; w<fontd.fontFileWidth; w++)
 			{
				unsigned int bitonepos;
				unsigned int bittwopos;
				switch(bit/2) {
					case 0:
						bitonepos=bit1;
						bittwopos=bit2;
					break;
					case 1:
						bitonepos=bit3;
						bittwopos=bit4;
					break;
					case 2:
						bitonepos=bit5;
						bittwopos=bit6;
					break;
					case 3:
						bitonepos=bit7;
						bittwopos=bit8;
					break;
				}
				unsigned char bitone=(unconvertedData[i]&bitonepos)>>(7-bit);
				unsigned char bittwo=(unconvertedData[i]&bittwopos)>>(6-bit);
	
				fontd.data[(h*fontd.fontFileWidth*4)+(4*w)]=(unsigned char)(bitone*r);
				fontd.data[(h*fontd.fontFileWidth*4)+(4*w)+1]=(unsigned char)(bitone*g);
				fontd.data[(h*fontd.fontFileWidth*4)+(4*w)+2]=(unsigned char)(bitone*b);
				fontd.data[(h*fontd.fontFileWidth*4)+(4*w)+3]=(unsigned char)(bittwo*255);
				
				bit+=2;
				if (bit>=8)
				{
					bit=0;
					i++;
				}
			}

			for (unsigned int p=0; p<padding*2; p++ )
			{
				bit++;
				if (bit>=8)
				{
					bit=0;
					i++;
				}
			}
		}

		// Convert into SDL surface
		temp = SDL_CreateRGBSurfaceFrom(fontd.data, fontd.fontFileWidth, fontd.fontFileHeight, 32, fontd.fontFileWidth*4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		font = SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);

		// Create SDL rects for each font character
		unsigned int y=0;
		i=0;
		while (y<fontd.fontFileHeight)
		{
			unsigned int x=0;
			while (x<fontd.fontFileWidth)
			{
				rects[i].x=x;
				rects[i].y=y;
				rects[i].w=fontd.charWidths[i];
				rects[i].h=static_cast<Uint16>(fontd.charHeight);
				x+=fontd.charWidths[i];
				i++;
			}
			y+=fontd.charHeight;
		}

		delete unconvertedData;

	} else 
	{
		throw "Error reading file\n";
	}
}

void Font::drawString(SDL_Surface *screen, int x, int y, char *text)
{
	std::string test=text;
	for (unsigned int i=0; i<test.length(); i++)
	{
		char currentChar=text[i];
		SDL_Rect pos;
		pos.x=x;
		pos.y=y;
		SDL_BlitSurface(font,&rects[currentChar],screen,&pos);
		x+=rects[currentChar].w;
		//y+=(rects[currentChar].y1-rects[currentChar].y);
	}
}

Font::~Font()
{
	delete fontd.charWidths;
	delete fontd.data;
	if (font!=NULL) SDL_FreeSurface(font);
}
