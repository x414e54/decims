#include "Decims.h"

Game::Game(Player *cp) {
	p=cp;
	phase=0;
	waiting=false;
	p2Ready=false;
}

int Game::ready()
{
	if (phase==0)
	{
		if (p->replaceDecim==NULL)
		{ 
			return -1; // Swap phase replace decim must not be NULL as it is to be passed to other player.
		}
		if (p2Ready)
		{
			waiting=true;
			return 1;
		}
	} else
	{
		if (p->replaceDecim!=NULL)
		{ 
			return -2; // Calculate phase - replace decim must be NULL
		}
		for (int k=0; k<4; k++)
		{
			if (p->hand[k]==NULL) { return -3; } // Missing Decim
		}
		if (p2Ready)
		{
			waiting=true;
			return 2;
		}
	}
	waiting=true;
	return 0;
}

int Game::otherReady()
{
	if (phase==0)
	{
		if (waiting && p->replaceDecim!=NULL)
		{
			p2Ready=true;
			return 0;
		} else 
		{
			p2Ready=true;
		}

	} else
	{
		if (waiting && p->replaceDecim==NULL)
		{
			p2Ready=true;
			return 1;
		} else 
		{
			p2Ready=true;
		}
	}
	return 2;
}

int Game::swap(int newDecim)
{
	if (phase==0)
	{
		if (waiting && p2Ready)
		{
			p->replaceDecim->setValue(newDecim);			
			waiting=false;
			p2Ready=false;
			phase=1;
			return 0;
		}
	}
	return -1;
}

int Game::endRound(int otherPlayerScore)
{
	if (phase==1)
	{
		if (waiting && p2Ready)
		{
			phase=0;
			waiting=false;
			p2Ready=false;
			int player1FinalHand=p->equateHand();
			int player2FinalHand=otherPlayerScore;
			int player1Diff;
			int player2Diff;
			if (player1FinalHand>=42)
			{
				player1Diff=player1FinalHand-42;
			} else {
				player1Diff=42-player1FinalHand;
			}
			if (player2FinalHand>=42)
			{	
				player2Diff=player2FinalHand-42;
			} else {
				player2Diff=42-player2FinalHand;
			}
		
			if (player1Diff<player2Diff)
			{
					p->clearBlib();
					p->buyBlib();
					p->newHand();
					if (p->complete())
					{
						return 3;
					} else
					{
						return 0;
					}
			} else if (player2Diff<player1Diff)
			{
					p->clearBlib();
					p->newHand();
					if (p->complete())
					{
						return 3;
					} else
					{
						return 1;
					}
			} else
			{
					p->clearBlib();
					p->newHand();
					if (p->complete())
					{
						return 3;
					} else
					{
						return 2;
					}
			}
		}
	}
	return -1;
}	

Game::~Game()
{
	p=NULL;
}