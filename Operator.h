class Operator {
public:
	int type;
	SDL_Surface *image;
	Operator(int t, const char* display);
	~Operator();
};