class Decim {
public:
	int id;
	int value;
	Decim(SDL_Surface *images[11]);
	void regenerate();
	void setValue(int val);
	SDL_Surface *image;
	SDL_Surface *images[11];
	~Decim();
};
