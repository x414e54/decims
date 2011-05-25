#include "Decims.h"

GUI::GUI() 
{
}

void GUI::addComponent(unsigned int index, void *comp)
{
	components.reserve(index+1);
	if (index > components.size() )
	{
		components.push_back(reinterpret_cast<GUIComponent *>(comp));
	} else
	{
		components.insert(components.begin()+index, reinterpret_cast<GUIComponent *>(comp));
	}
}

GUIComponent* GUI::getComponent(unsigned int index) 
{
	if (index<components.size())
	{
		return components.at(index);
	} else
	{
		return NULL;
	}
}

Static * GUI::getStatic(unsigned int index)
{
	return static_cast<Static *>(getComponent(index));
}

ComboBox *GUI::getComboBox(unsigned int index)
{
	return static_cast<ComboBox *>(getComponent(index));
}

TextBox *GUI::getTextBox(unsigned int index)
{
	return static_cast<TextBox *>(getComponent(index));
}

TextList *GUI::getTextList(unsigned int index)
{
	return static_cast<TextList *>(getComponent(index));
}

DragImageSet *GUI::getDragImageSet(unsigned int index)
{
	return static_cast<DragImageSet *>(getComponent(index));
}

Button *GUI::getButton(unsigned int index)
{
	return static_cast<Button *>(getComponent(index));
}

ToggleImage *GUI::getToggleImage(unsigned int index)
{
	return static_cast<ToggleImage *>(getComponent(index));
}

void GUI::render(SDL_Surface *screen)
{
	for (unsigned int i=0; i<components.size(); i++)
	{
		if (components.at(i)!=NULL) { components.at(i)->render(screen); }
	}
}

void GUI::action(SDL_Event event)
{
	for (unsigned int i=0; i<components.size(); i++)
	{
		if (components.at(i)!=NULL) { components.at(i)->action(event); }
	}
}

GUI::~GUI()
{
	for (unsigned int i=0; i<components.size(); i++)
	{
		delete components.at(i);
	}
}