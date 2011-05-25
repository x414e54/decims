class Game {
public:
	Game(Player *p);
	int endRound(int value);
	Player *p;
	int phase;
	bool waiting;
	bool p2Ready;
	int ready();
	int otherReady();
	int swap(int newDecim);
	~Game();
};