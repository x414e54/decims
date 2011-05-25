#include "Decims.h"

Player::Player()
{
	replaceDecim=NULL;
	blib=NULL;
	for (int i=0; i<10; i++)
	{
		set[i]=false;
	}
}
void Player::newHand()
{
	for (int i=0;i<4;i++)
	{
		hand[i]->regenerate();
	}
}

void Player::swap(Player *p)
{
	Decim *tmp =p->replaceDecim;
	p->replaceDecim=replaceDecim;
	replaceDecim=tmp;
}

int Player::equateHand()
{
	if (hand[0]==NULL) { return 0; }
	int equate=hand[0]->value;
	for (int i=0; i<3; i++)
	{
		if (hand[i+1]==NULL) { return 0; }
		switch(operators[i]->type)
		{
			case 0:
				equate/=hand[i+1]->value;
			break;
			case 1:
				equate*=hand[i+1]->value;
			break;
			case 2:
				equate-=hand[i+1]->value;
			break;
			case 3:
				equate+=hand[i+1]->value;
			break;
		}
	}
	return equate;
}

void Player::selectDecim(int index)
{
	if (index>=-1 && index<5)
	{
		selectedDecim=index;
	}
}

void Player::moveDecim(int index)
{
	if (index!=selectedDecim)
	{
		if (index==-1)
		{
			Decim *tmp=blib;
			blib=hand[selectedDecim];
			hand[selectedDecim]=tmp;
		} else if (index>=0 && index <4)
		{
			Decim *tmp=hand[index];
			if (selectedDecim==-1)
			{
				hand[index]=blib;
				blib=tmp;
			} else if (selectedDecim==4)
			{
				hand[index]=replaceDecim;
				replaceDecim=NULL;
			} else
			{
				hand[index]=hand[selectedDecim];
				hand[selectedDecim]=tmp;
			}
		} else if (index==4)
		{
			if (selectedDecim>=1 && selectedDecim<4)
			{
				replaceDecim=hand[selectedDecim];
				hand[selectedDecim]=NULL;
			}
		}
	}
}

void Player::buyBlib()
{
	blib=blibHidden;
	blibHidden=NULL;
	blib->regenerate();
}

void Player::setOperator(Operator *op, int index)
{
	if (index>=0 && index<3)
	{
		operators[index]=op;
	}
}

void Player::clearBlib()
{
	if (blib!=NULL)
	{
		set[blib->value-1]=true;
		blibHidden=blib;
		blib=NULL;
	}
}

bool Player::complete()
{
	bool complete=true;
	for (int i=0; i<10; i++)
	{
		if (set[i]==false) {complete=false; break; }
	}
	return complete;
}

Player::~Player()
{
	for (int i=0; i<4; i++)
	{
		delete hand[i];
		hand[i]=NULL;
	}
	delete blib;
	blib=NULL;
	delete blibHidden;
	blibHidden=NULL;
	delete replaceDecim;
	replaceDecim=NULL;
}