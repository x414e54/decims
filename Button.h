
////////////////////////////////// Button.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class Button : public GUIComponent
{
public:
	SDL_Surface *onimg;
	SDL_Surface *offimg;
	char *text;
	Font *f;
	int x;
	int y;
	bool pressed;
	bool actionready;
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	bool hasAction();
	Button(int x, int y, const char* text, Font *f, const char* on, const char* off);
	~Button();
};
