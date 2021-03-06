
////////////////////////////////// Static.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class Static : public GUIComponent {
public:
	char* text;
	int x;
	int y;
	Font *f;
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	void setText(const char *text);
	Static(int x, int y, const char* text, Font *f);
	~Static();
};

