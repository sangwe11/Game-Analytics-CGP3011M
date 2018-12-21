#include "Events.h"

Events::Events()
{
	m_keystate = SDL_GetKeyboardState(NULL);
}

void Events::update()
{
	SDL_Event e;

	m_events.clear();
	m_mousestate.reset();
	m_keysDown.clear();
	m_keysUp.clear();

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_alive = false;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			m_events[SDL_KEYDOWN] = true;
			m_keysDown[e.key.keysym.scancode] = true;
		}
		else if (e.type == SDL_KEYUP)
		{
			m_events[SDL_KEYUP] = true;
			m_keysUp[e.key.keysym.scancode] = true;
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			m_events[SDL_MOUSEMOTION] = true;

			m_mousestate.x = e.motion.x;
			m_mousestate.y = e.motion.y;
			m_mousestate.xrel = e.motion.xrel;
			m_mousestate.yrel = e.motion.yrel;
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			m_events[SDL_MOUSEWHEEL] = true;
			m_mousestate.scroll = e.wheel.y;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			m_events[SDL_MOUSEBUTTONDOWN] = true;
			m_mousestate.buttons[e.button.button] = true;
			m_mousestate.heldButtons[e.button.button] = true;
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			m_events[SDL_MOUSEBUTTONUP] = true;
			m_mousestate.buttons[e.button.button] = false;
			m_mousestate.heldButtons[e.button.button] = false;
		}
	}
}

const Uint8 *Events::keyState()
{
	return m_keystate;
}

bool Events::checkEvent(SDL_EventType e)
{
	return m_events[e];
}

bool Events::isAlive()
{
	return m_alive;
}

bool Events::checkKeyPress(SDL_Scancode key)
{
	return m_keysDown[key];
}

bool Events::checkKeyRelease(SDL_Scancode key)
{
	return m_keysUp[key];
}

MouseState &Events::mouseState()
{
	return m_mousestate;
}