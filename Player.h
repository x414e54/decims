
class Player {
public:
	Decim *hand[4];
	Decim *blib;
	Decim *blibHidden;
	Decim *replaceDecim;
	bool set[10];
	Operator *operators[3];
	int selectedDecim;
	void selectDecim(int index);
	void moveDecim(int index);
	void newHand();
	int equateHand();
	void buyBlib();
	void setOperator(Operator *op, int index);
	void swap(Player *p);
	bool complete();
	void clearBlib();
	Player();
	~Player();
};