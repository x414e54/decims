
////////////////////////////////// ToggleImage.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class ToggleImage : public GUIComponent {
public:
	SDL_Surface *off;
	SDL_Surface *on;
	int x;
	int y;
	bool btoggle;
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	void toggle();
	bool toggled();
	ToggleImage(int x, int y, const char *on, const char *off);
	~ToggleImage();
};
