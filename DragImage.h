
////////////////////////////////// DragImage.h ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class DragImage;

class DragImageSet : public GUIComponent {
public:
	DragImageSet();
	void addDragImage(DragImage *image);
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	bool drop(SDL_Event event);
	DragImage *getDragImage(unsigned int index);
	std::vector<DragImage*> imageset;
	bool dragging;
	~DragImageSet();
	DragImage *drag;
};

class DragImage : public GUIComponent {
public:
	DragImage(int x, int y, SDL_Surface *image, DragImageSet *set);
	void action(SDL_Event event);
	void render(SDL_Surface *screen);
	void setDecim(Decim **decim);
	SDL_Surface *image;
	bool dragging;
	int x;
	int startx;
	int y;
	int starty;
	DragImageSet *set;
	Decim **decim;
	bool movefrom;
	bool moveto;
	~DragImage();
};
