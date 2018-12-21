#include "Display.h"

#include <GL\glew.h>
#include <iostream>

Display::Display(const std::string &title, int width, int height)
{
	m_width = width;
	m_height = height;
	m_title = title;

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	m_id = SDL_GetWindowID(m_window);
	m_context = SDL_GL_CreateContext(m_window);

	glewExperimental = GL_TRUE;
	GLenum glew = glewInit();

	if (glew != GLEW_OK)
	{
		std::cerr << "Glew init failed." << std::endl;
		std::cerr << glewGetErrorString(glew) << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "Glew init." << std::endl;
	}

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display::update()
{

}

void Display::swap()
{
	SDL_GL_SwapWindow(m_window);
}

void Display::clear(float r, float g, float b, float a, float d)
{
	glClearColor(r, g, b, a);
	glClearDepth(d);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}