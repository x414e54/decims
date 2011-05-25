
////////////////////////////////// TextList.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class TextList : public GUIComponent {
public:
	char text[5][512];
	int start;
	int pos;
	int elements;
	int x;
	int y;
	Font *f;
	void addText(char *buffer);
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	TextList(int x, int y, Font *f);
	~TextList();
};
