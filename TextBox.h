////////////////////////////////// TextBox.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class TextBox : public GUIComponent {
public:
	char* text;
	int pos;
	int x;
	int y;
	Font *f;
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	void setText(const char* ctext);
	const char *getText();
	TextBox(int x, int y, Font *f);
	~TextBox();
};
