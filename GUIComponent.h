///////////////////////////////// GUIComponent.h /////////////////////////////////////////////////
////////////////////////////////////// Virtual/abstract class for components of the GUI //////////

class GUIComponent {
public:
	virtual void action(SDL_Event event) = 0 ;
	virtual void render(SDL_Surface *screen) = 0;
};
