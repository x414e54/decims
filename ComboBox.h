
////////////////////////////////// ComboBox.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class ComboBox : public GUIComponent {
public:
	ComboBox(int x, int y);
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	void addOption(Operator * op);
	void setSelected(Operator **selected);
	std::vector <Operator *> options;
	Operator **selected;
	int x;
	int y;
	bool open;
	~ComboBox();
};
