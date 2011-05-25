typedef unsigned short WORD;
typedef unsigned int  DWORD;

struct FontData
{   
  DWORD fontFileWidth;       
  DWORD fontFileHeight;       
  unsigned char *charWidths;
  DWORD charHeight;
  WORD	fontSig;
  unsigned char *data;
};

class Font {
public:
	Font(const char *fontfile, int r, int g, int b);
	void drawString(SDL_Surface *screen, int x, int y, char *text);
	SDL_Surface *font; 
	FontData fontd; 
	SDL_Rect rects[256];
	~Font();
};
