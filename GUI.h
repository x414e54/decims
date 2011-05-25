
////////////////////////////////// GUI.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class GUI : public GUIComponent {
public:
	std::vector<GUIComponent *> components;
	GUI();
	void addComponent(unsigned int index, void *comp);
	GUIComponent* getComponent(unsigned int index);
	Static * getStatic(unsigned int index);
	ComboBox *getComboBox(unsigned int index);
	TextBox *getTextBox(unsigned int index);
	TextList *getTextList(unsigned int index);
	DragImageSet *getDragImageSet(unsigned int index);
	Button *getButton(unsigned int index);
	ToggleImage *getToggleImage(unsigned int index);
	void render(SDL_Surface *screen);
	void action(SDL_Event event);
	~GUI();
};