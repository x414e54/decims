class DecimEngine {
public:
	TCPsocket sd;
	TCPsocket csd;
	SDLNet_SocketSet socks; 
	IPaddress ip;
	IPaddress *remoteIP;
	char buffer[512];
	SDL_Event event;
	SDL_Surface *bg;
	int pos;
	SDL_Surface* screen;
	Font *f;
	bool msg;
	bool waiting;
	bool endGame;
	int phase;

	Player *p;
	Game *g;
	Operator *op1;
	Operator *op2;
	Operator *op3;
	Operator *op4;
	GUI *gui;
	GUI *setgui;
	GUI *msggui;
	GUI *endGamegui;
	GUI *waitinggui;
	GUI *quitgui;
	bool server;
	int hostOrJoin;
	const char *hostname;
	const char *name;
	const char *decimimagestrings[31];
	SDL_Surface *decimimages[11];

	DecimEngine(const char *imgstrings[]);
	DecimEngine(const char *hostname, const char *imgstrings[]);

private:
	int renderLoop();
	int eventLoop();
	int initNetwork();
	int initVideo();
	int networkLoop();
	int gameLoop();
	int finalCleanup();
	int start();
};