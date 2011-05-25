
#include "Decims.h"

#define CMP_MSGENTER				0
#define CMP_MSGS					1
#define CMP_MSGENTERSTATIC			2
#define CMP_PHASESTATIC				3
#define CMP_SWAPSTATIC				4
#define CMP_BLIBSTATIC				5
#define CMP_EQIMAGE					6
#define CMP_READYBUTTON				7
#define CMP_OP1						8
#define CMP_OP2						9
#define CMP_OP3						10
#define CMP_EQSTATIC				11
#define CMP_DECIMS					12
#define CMP_MSGIMAGE				0
#define CMP_MSGSTATIC				1
#define CMP_OKAYBUTTON				2
#define CMP_WAITINGIMAGE			0
#define CMP_WAITINGSTATIC			1
#define CMP_QUITBUTTON				0

#define DEFAULT_PORT				8013

/* In general all functions will
	return 1 on normal completion
	return 0 if an exit/quit request is made
	throw on an error*/

DecimEngine::DecimEngine(const char *chostname, const char *imgstrings[31])
{
	for (int i=0; i<31; i++)
	{
		decimimagestrings[i]=imgstrings[i];
	}
	server=false;
	msg=false;
	waiting=false;
	endGame=false;
	hostname=chostname;
	phase=0;
	start();
}

DecimEngine::DecimEngine(const char *imgstrings[31])
{
	for (int i=0; i<31; i++)
	{
		decimimagestrings[i]=imgstrings[i];
	}
	server=true;
	msg=false;
	waiting=false;
	endGame=false;
	phase=0;
	start();
}

int DecimEngine::initVideo()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		throw SDL_GetError();
	}

	screen = SDL_SetVideoMode(640, 480, 0, 0);
	SDL_WM_SetCaption("Decims","Decims");

	SDL_Surface* temp = SDL_LoadBMP(decimimagestrings[17]);
	if (temp==NULL) throw "Error reading file\n";
	bg = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	f = new Font("media/font.aqf",29, 226, 187);
			
	for (int i=0; i<11; i++)
	{
		SDL_Surface* temp = SDL_LoadBMP(decimimagestrings[i]);
		if (temp==NULL) throw "Error reading file\n";
		decimimages[i] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}
	
	// Initialize player and the player's cards (Decims)
	p = new Player();
	p->blibHidden=new Decim(decimimages);
	p->hand[0]=new Decim(decimimages);
	p->hand[1]=new Decim(decimimages);
	p->hand[2]=new Decim(decimimages);
	p->hand[3]=new Decim(decimimages);

	// Initlize the useable mathmatical operators that are usable in this game.
	op1 = new Operator(0, decimimagestrings[11]);
	op2 = new Operator(1, decimimagestrings[12]);
	op3 = new Operator(2, decimimagestrings[13]);
	op4 = new Operator(3, decimimagestrings[14]);

	// Set Player's Default operators (add is the default).
	p->setOperator(op4,0);
	p->setOperator(op4,1);
	p->setOperator(op4,2);

	// Create game
	g = new Game(p);

	// GUI for holding most of the main functionality of the game, anything that will always be rendered can go here
	gui = new GUI();

	gui->addComponent(CMP_EQIMAGE, new Image(476,310,decimimagestrings[15]));

	gui->addComponent(CMP_MSGENTER, new TextBox(100,0,f));
	gui->addComponent(CMP_MSGS, new TextList(100,0,f));
	gui->addComponent(CMP_MSGENTERSTATIC, new Static(0, 70, "Enter Message to send:",f));
	gui->addComponent(CMP_PHASESTATIC, new Static(10, 230, "Swap Phase - Please Place a Decim in the swap spot that you would like to swap.",f));
	gui->addComponent(CMP_SWAPSTATIC, new Static(118, 210, "(Swap)",f));
	gui->addComponent(CMP_BLIBSTATIC, new Static(418, 210, "(Blib)",f));

	gui->addComponent(CMP_READYBUTTON, new Button(320,430, "Ready", f, decimimagestrings[18], decimimagestrings[19]));

	gui->addComponent(CMP_OP1, new ComboBox(176,310));
	gui->getComboBox(CMP_OP1)->addOption(op1);
	gui->getComboBox(CMP_OP1)->addOption(op2);
	gui->getComboBox(CMP_OP1)->addOption(op3);
	gui->getComboBox(CMP_OP1)->addOption(op4);
	gui->getComboBox(CMP_OP1)->setSelected(&p->operators[0]);
			
	gui->addComponent(CMP_OP2, new ComboBox(276,310));
	gui->getComboBox(CMP_OP2)->addOption(op1);
	gui->getComboBox(CMP_OP2)->addOption(op2);
	gui->getComboBox(CMP_OP2)->addOption(op3);
	gui->getComboBox(CMP_OP2)->addOption(op4);
	gui->getComboBox(CMP_OP2)->setSelected(&p->operators[1]);

	gui->addComponent(CMP_OP3, new ComboBox(376,310));
	gui->getComboBox(CMP_OP3)->addOption(op1);
	gui->getComboBox(CMP_OP3)->addOption(op2);
	gui->getComboBox(CMP_OP3)->addOption(op3);
	gui->getComboBox(CMP_OP3)->addOption(op4);
	gui->getComboBox(CMP_OP3)->setSelected(&p->operators[2]);

	gui->addComponent(CMP_EQSTATIC, new Static(510, 320, "",f));

	gui->addComponent(CMP_DECIMS,new DragImageSet());
	DragImageSet *imageset = gui->getDragImageSet(CMP_DECIMS);
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(110,110,decimimages[10],imageset));
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(410,110,decimimages[10],imageset));
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(110,280,decimimages[10],imageset));
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(210,280,decimimages[10],imageset));
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(310,280,decimimages[10],imageset));
	gui->getDragImageSet(CMP_DECIMS)->addDragImage(new DragImage(410,280,decimimages[10],imageset));

	gui->getDragImageSet(CMP_DECIMS)->getDragImage(0)->setDecim(&p->replaceDecim);
	gui->getDragImageSet(CMP_DECIMS)->getDragImage(1)->setDecim(&p->blib);
	gui->getDragImageSet(CMP_DECIMS)->getDragImage(2)->setDecim(&p->hand[0]);
	gui->getDragImageSet(CMP_DECIMS)->getDragImage(3)->setDecim(&p->hand[1]);
	gui->getDragImageSet(CMP_DECIMS)->getDragImage(4)->setDecim(&p->hand[2]);
	gui->getDragImageSet(CMP_DECIMS)->getDragImage(5)->setDecim(&p->hand[3]);

	/* GUI for the message box that will pop up stating information to the player,
		rendered on msg=true */
	msggui = new GUI();
	msggui->addComponent(CMP_MSGIMAGE, new Image(150,150,decimimagestrings[16]));
	msggui->addComponent(CMP_MSGSTATIC, new Static(200, 200, "Error",f));
	msggui->addComponent(CMP_OKAYBUTTON, new Button(300, 300, "Okay", f, decimimagestrings[18], decimimagestrings[19]));

	/* GUI for the message box that will pop up stating we are waiting for the other player to finish their
		move,
		rendered on waiting=true */
	waitinggui = new GUI();
	waitinggui->addComponent(CMP_WAITINGIMAGE, new Image(150, 150,decimimagestrings[16]));
	waitinggui->addComponent(CMP_WAITINGSTATIC, new Static(200, 200, "Waiting for Other Player...",f));

	/* GUI for the message box that will pop up covering the main gui stating the game has finished
		rendered on endGame=true */
	endGamegui = new GUI();
	endGamegui->addComponent(CMP_WAITINGIMAGE, new Image(150, 150,decimimagestrings[16]));
	endGamegui->addComponent(CMP_WAITINGSTATIC, new Static(300, 250, "Game Over",f));

	/* GUI for the sets, to display if a user has "won" that decim, always rendred could go in the 
		main gui, goes here for tidyness*/
	setgui = new GUI();
	setgui->addComponent(0, new ToggleImage(10, 420, decimimagestrings[20], decimimagestrings[30])); 
	setgui->addComponent(1, new ToggleImage(40, 420, decimimagestrings[21], decimimagestrings[30]));
	setgui->addComponent(2, new ToggleImage(70, 420, decimimagestrings[22], decimimagestrings[30]));
	setgui->addComponent(3, new ToggleImage(100, 420, decimimagestrings[23], decimimagestrings[30]));
	setgui->addComponent(4, new ToggleImage(130, 420, decimimagestrings[24], decimimagestrings[30]));
	setgui->addComponent(5, new ToggleImage(160, 420, decimimagestrings[25], decimimagestrings[30]));
	setgui->addComponent(6, new ToggleImage(190, 420, decimimagestrings[26], decimimagestrings[30]));
	setgui->addComponent(7, new ToggleImage(220, 420, decimimagestrings[27], decimimagestrings[30]));
	setgui->addComponent(8, new ToggleImage(250, 420, decimimagestrings[28], decimimagestrings[30]));
	setgui->addComponent(9, new ToggleImage(280, 420, decimimagestrings[29], decimimagestrings[30]));

	// Quit button that is always displayed and clickable
	quitgui = new GUI();
	quitgui->addComponent(CMP_QUITBUTTON, new Button(420,430, "QUIT", f, decimimagestrings[18], decimimagestrings[19]));
	return 1;
}

int DecimEngine::renderLoop()
{
	SDL_BlitSurface(bg,NULL,screen,NULL);

	quitgui->render(screen);

	setgui->render(screen);

	gui->render(screen);

	if (endGame)
	{
		endGamegui->render(screen);
	}

	if (msg)
	{
		msggui->render(screen);
	}

	if (waiting)
	{
		waitinggui->render(screen);
	}

	SDL_UpdateRect(screen,0, 0, 0, 0);
	return 1;
}

int DecimEngine::eventLoop()
{
	char equated[10];
	//_itoa(p->equateHand(),equated,10);
	snprintf(equated, 10, "%d", p->equateHand());
	gui->getStatic(CMP_EQSTATIC)->setText(equated); // Equate player's hand and set the text to it.

	if (phase!=g->phase)
	{
		phase=g->phase;
		if (g->phase==0)
		{
			gui->getStatic(CMP_PHASESTATIC)->setText("Swap Phase - Please Place a Decim in the swap spot that you would like to swap.");
		} else 
		{
			gui->getStatic(CMP_PHASESTATIC)->setText("Calculate Phase - Change operators to get as close to the target.");
		}
	}

	for (int i=0; i<10; i++)
	{
		if (p->set[i]!=setgui->getToggleImage(i)->toggled()) setgui->getToggleImage(i)->toggle(); // Set the "set" images to on if they player has won that "decim"
	}

	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				{
					char sendbuff[512];
					sendbuff[0]=4;
					SDLNet_TCP_Send(csd, sendbuff, 512);
					return 0; break;
				}
			case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					{
						char sendbuff[512];
						sendbuff[0]=4;
						SDLNet_TCP_Send(csd, sendbuff, 512);
						return 0; break;
					}
				case SDLK_RETURN:
				{	
					SDLNet_TCP_Send(csd, (void *)gui->getTextBox(CMP_MSGENTER)->getText(), 512);
					gui->getTextBox(CMP_MSGENTER)->setText("");
					break;
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
				break;
				case SDL_BUTTON_RIGHT:
				break;
			}
			break;
			case SDL_MOUSEMOTION:
			break;
		}

		quitgui->action(event);

		if (quitgui->getButton(CMP_QUITBUTTON)->hasAction())
		{ 
			char sendbuff[512];
			sendbuff[0]=4;
			SDLNet_TCP_Send(csd, sendbuff, 512);
			return 0;
		}				

		if (!waiting)
		{
			if (msg)
			{
				msggui->action(event);
				if (msggui->getButton(CMP_OKAYBUTTON)->hasAction()) { msg=false; }				
			} else
			{
				if (endGame) { return 1; }
				gui->action(event);
				if (gui->getButton(CMP_READYBUTTON)->hasAction())
				{ 
					int res=g->ready();
					switch (res)
					{
						case 0:					// We are ready, no problems and other play has not told us they are ready - notify other player
							char sendbuff[512];
							sendbuff[0]=0;
							SDLNet_TCP_Send(csd, sendbuff, 512);
							waiting=true;
						break;
						case -1:				// We have not selected a decim to swap and are in the swap phase
						{
							msggui->getStatic(CMP_MSGSTATIC)->setText("Select a Decim to swap.");
							msg=true;
						}
						break;
						case -2:				// Non-Swap phase and still have a decim in the swap spot
						{
							msggui->getStatic(CMP_MSGSTATIC)->setText("You must empty your swap space.");
							msg=true;
						}
						break;
						case -3:				// We are missing a decim in our hand, someone may have placed it in the blib spot without having the extra decim
						{
							msggui->getStatic(CMP_MSGSTATIC)->setText("You have a NULL decim in your hand.");
							msg=true;
						}
						break;
						case 1:					// Swap phase, no problems, and the other player is ready, send our readyness and swap value
						{
							char sendbuff[512];
							sendbuff[0]=0;
							SDLNet_TCP_Send(csd, sendbuff, 512);
							waiting=true;
							sendbuff[0]=1;
							sendbuff[1]=(char)p->replaceDecim->value;
							SDLNet_TCP_Send(csd, sendbuff, 512); 
						}
						break;
						case 2:					// Non-Swap phase, no problems, and the other player is ready, send our readyness and FINAL value
						{
							char sendbuff[512];
							sendbuff[0]=0;
							SDLNet_TCP_Send(csd, sendbuff, 512);
							waiting=true;
							sendbuff[0]=2;
							int value = p->equateHand();
							char *p = reinterpret_cast<char *>(&value);
							for (int i=0; i<4; i++)
							{
								sendbuff[i+1]=*p;
								p++;
							}
							SDLNet_TCP_Send(csd, sendbuff, 512); 
						}
						break;
					}
				}
			}
		}
	}
	return 1;
}

int DecimEngine::initNetwork()
{
	if (SDLNet_Init() < 0)
	{
		throw SDLNet_GetError();
	}

 	const char *tmphostname;
	if (server)
	{
		tmphostname=NULL;		// If server no hostname is required instead NULL must be sent
	} else
	{
		tmphostname=hostname;	// If client we need a host name to connect to server
	}
	
	if (SDLNet_ResolveHost(&ip, tmphostname, DEFAULT_PORT) < 0)
	{
		throw SDLNet_GetError();
	}
 
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		throw SDLNet_GetError();
	}

	socks=SDLNet_AllocSocketSet(1);

	return 1;
}

int DecimEngine::networkLoop()
{
	if (SDLNet_CheckSockets(socks,1))				// Has anything been sent to our socket?
	{
		if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
		{
			switch (buffer[0]) {					// What message has been sent?
				case 0:								// - Player is ready
				{
					int res = g->otherReady();
					switch (res) // Are we ready and what stage are we in?
					{
						case 0: // Stage 0, and we are ready - Send swap
						{
							char sendbuff[512];
							sendbuff[0]=1;
							sendbuff[1]=(char)p->replaceDecim->value;
							SDLNet_TCP_Send(csd, sendbuff, 512); 
						}
						break;
						case 1: // Stage 1, and we are ready - Announce value
						{
							char sendbuff[512];
							int value = p->equateHand();
							char *p = reinterpret_cast<char *>(&value);
							sendbuff[0]=2;
							for (int i=0; i<4; i++)
							{
								sendbuff[i+1]=*p;
								p++;
							}
							SDLNet_TCP_Send(csd, sendbuff, 512); 
						}
						break;
					}
				}
				break;
				case 1:								// Player has send a Swap value
					if (g->swap((int)buffer[1])==0) { // Is player ready and are we in the right phase?
						waiting=false;
					}
				break;
				case 2:								// Player has sent their final value
				{
					char arr[4];
					arr[0]=buffer[1];
					arr[1]=buffer[2];
					arr[2]=buffer[3];
					arr[3]=buffer[4];
					int *p=reinterpret_cast<int *>(arr);

					switch (g->endRound(*p)) { // Is player ready and are we in the right phase? What happens
						case 0: // We win
							waiting=false;
							msggui->getStatic(CMP_MSGSTATIC)->setText("You Win Round!"); 
							msg=true;
						break;
						case 1: // Other player wins
							waiting=false;
							msggui->getStatic(CMP_MSGSTATIC)->setText("Other player wins round!");
							msg=true;
						break;
						case 2: // Both values were equal
							waiting=false;
							msggui->getStatic(CMP_MSGSTATIC)->setText("Draw!");
							msg=true;
						break;
						case 3: // We have filled out sets with "blibs", we win the game
							waiting=false;
							msggui->getStatic(CMP_MSGSTATIC)->setText("You Win Game!");
							msg=true;
							char sendbuff[512];										// Notify our player of completion
							sendbuff[0]=3;
							SDLNet_TCP_Send(csd, sendbuff, 512);
							endGame=true;
						break;
					}
				}
				break;
				case 3:	// Other player notifing us that they have completed their set of blibs
					/* For the simpleness we will assume that they only send this value if they have actually completed
					their sets and no failures have happend on the network. This is a naieve assumption. */
					msggui->getStatic(CMP_MSGSTATIC)->setText("You have lost the Game!");
					msg=true;
					endGame=true;
				break;
				case 4: // Other player notifing us that they have pressed their quit button.
					msggui->getStatic(CMP_MSGSTATIC)->setText("Client has quit!");
					msg=true;
					endGame=true;
				break;
				default: gui->getTextList(CMP_MSGS)->addText(buffer); // Nothing matched therefore it must just be a message in text, add to TextList.
				break;
			}
		}
	}
	return 1;
}

int DecimEngine::gameLoop()
{
	if (!server || (csd = SDLNet_TCP_Accept(sd)))
	{
		if (server)
		{
			SDLNet_AddSocket(socks, (SDLNet_GenericSocket)csd);
		} else
		{
			SDLNet_AddSocket(socks, (SDLNet_GenericSocket)sd);
			csd=sd;
		}

		if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
		{
			printf("Connected\n");
		} else
		{
			throw "No peer address";
		}
	
		initVideo();

		pos=0;

		bool quit = false;

		while (!quit)
		{
			if (eventLoop()!=1) return 0;
			if (renderLoop()!=1) return 0;
			if (networkLoop()!=1) return 0;
		}

		SDLNet_TCP_Close(csd);
	}
	return 1;
}

int DecimEngine::finalCleanup()
{
	if (bg!=NULL) { SDL_FreeSurface(bg); }
	if (screen!=NULL) { SDL_FreeSurface(screen); }
	for (int i=0; i<11; i++)
	{
		if (decimimages!=NULL) { SDL_FreeSurface(decimimages[i]); }
	}
	delete f;
	delete gui;
	delete setgui;
	delete msggui;
	delete endGamegui;
	delete waitinggui;
	delete quitgui;
	delete p;
	delete g;
	delete op1;
	delete op2;
	delete op3;
	delete op4;
	SDL_Quit();
	if (sd!=NULL) { SDLNet_TCP_Close(sd); }
	if (csd!=NULL) { SDLNet_TCP_Close(csd); }
	SDLNet_Quit(); 
	return 1;
}

int DecimEngine::start()
{
	try
	{
		initNetwork();
		bool quit=false;
		while(!quit)
		{
			if (gameLoop()!=1) quit=true;
		}
		finalCleanup();
		return 1;
	} catch (const char *e)
	{
		fprintf(stderr, "Error: %s", e);
		finalCleanup();
		exit(-1);
	}
}
