#ifndef _EVENTS_H
#define _EVENTS_H

#include <map>
#include <SDL\SDL.h>

struct MouseState
{
	int x;
	int y;
	int xrel = 0;
	int yrel = 0;
	int scroll = 0;
	std::map<Uint8, bool> buttons;
	std::map<Uint8, bool> heldButtons;

	void reset()
	{
		xrel = 0;
		yrel = 0;
		scroll = 0;
		buttons.clear();
	}
};

class Events
{
public:
	Events();

	void update();

	const Uint8 *keyState();
	bool checkEvent(SDL_EventType e);
	bool isAlive();

	bool checkKeyPress(SDL_Scancode key);
	bool checkKeyRelease(SDL_Scancode key);

	MouseState &mouseState();

private:
	const Uint8 *m_keystate;

	MouseState m_mousestate;
	bool m_alive = true;
	std::map<SDL_EventType, bool> m_events;
	std::map<SDL_Scancode, bool> m_keysDown;
	std::map<SDL_Scancode, bool> m_keysUp;
};

#endif