#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL\SDL.h>

class Display
{
public:
	Display(const std::string &title, int width, int height);
	~Display();

	const int width() { return m_width; }
	const int height() { return m_height; }

	void update();
	void swap();
	void clear(float r, float g, float b, float a, float d);

private:
	int m_id;
	SDL_Window* m_window;
	SDL_GLContext m_context;

	std::string m_title;
	int m_width;
	int m_height;

	void windowResized(int w, int h);
};

#endif

