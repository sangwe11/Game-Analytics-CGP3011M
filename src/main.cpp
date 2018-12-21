#define GLM_FORCE_RADIANS

#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

#include "FileIO.h"
#include "Events.h"
#include "Display.h"
#include "Shader.h"
#include "Trajectory.h"
#include "Heatmap.h"
#include "Font.h"
#include "Camera.h"
#include "PositionHeatmap.h"

FT_Library ft;
LogData *d1, *d2, *d3, *d4;
Trajectory *t1, *t2, *t3, *t4;
Heatmap *h1, *h2, *h3, *h4;
PositionHeatmap *p1;

int mode = 0;
int player = 1;
float currentPoint = 1.0f;
bool playMode = false;

void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL init failed." << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "SDL init." << std::endl;
	}

	if (FT_Init_FreeType(&ft))
	{
		std::cerr << "FT init failed." << std::endl;
		exit(1);
	}
}

void uninitialise()
{
	SDL_Quit();
}

int getCurrentFrame(int totalFrames, float percentage)
{
	percentage = glm::clamp(percentage, 0.0f, 100.0f);

	float lerped = glm::lerp(0.0f, (float)totalFrames, percentage);
	int rounded = (int)std::round(lerped);

	return rounded;
}

void changeMode(int newMode)
{
	mode = newMode;
	currentPoint = 100.0f;
	playMode = false;
}

void renderHeatmaps(int players, Camera &camera, float currentPoint)
{
	if (players == 5)
	{
		h1->draw(camera, getCurrentFrame(h1->getFrames(), currentPoint));
		h2->draw(camera, getCurrentFrame(h2->getFrames(), currentPoint));
		h3->draw(camera, getCurrentFrame(h3->getFrames(), currentPoint));
		h4->draw(camera, getCurrentFrame(h4->getFrames(), currentPoint));
	}
	else if (players == 4)
	{
		h4->draw(camera, getCurrentFrame(h4->getFrames(), currentPoint));
	}
	else if (players == 3)
	{
		h3->draw(camera, getCurrentFrame(h3->getFrames(), currentPoint));
	}
	else if (players == 2)
	{
		h2->draw(camera, getCurrentFrame(h2->getFrames(), currentPoint));
	}
	else if (players == 1)
	{
		h1->draw(camera);
	}
}

void renderTrajectory(int players, Camera &camera, float currentPoint, bool speed)
{
	if (players == 5)
	{
		t1->draw(camera, speed, getCurrentFrame(t1->getFrames(), currentPoint));
		t2->draw(camera, speed, getCurrentFrame(t2->getFrames(), currentPoint));
		t3->draw(camera, speed, getCurrentFrame(t3->getFrames(), currentPoint));
		t4->draw(camera, speed, getCurrentFrame(t4->getFrames(), currentPoint));
	}
	if (players == 4)
	{
		t4->draw(camera, speed, getCurrentFrame(t4->getFrames(), currentPoint));
	}
	else if (players == 3)
	{
		t3->draw(camera, speed, getCurrentFrame(t3->getFrames(), currentPoint));
	}
	else if (players == 2)
	{
		t2->draw(camera, speed, getCurrentFrame(t2->getFrames(), currentPoint));
	}
	else if (players == 1)
	{
		t1->draw(camera, speed, getCurrentFrame(t1->getFrames(), currentPoint));
	}
}

void renderCombined(int players, Camera &camera, float currentPoint, bool speed)
{
	renderHeatmaps(players, camera, currentPoint);
	renderTrajectory(players, camera, currentPoint, speed);
}

void renderPositionHeatmap(Camera &camera, float zoomLevel)
{
	p1->draw(camera, zoomLevel);
}

int main(int argc, char* args[])
{
	initialise();

	Events events;
	Display display("GameAnalytics", 1000, 1000);
	Camera camera(glm::vec3(0.035f, 0.215f, 63.0f), glm::vec3(0.0f, 0.0f, 0.0f), 80.0f, 0.01f, 1000.0f, display.width(), display.height());

	d1 = new LogData(parseLogFile("player1.txt"));
	d2 = new LogData(parseLogFile("player2.txt"));
	d3 = new LogData(parseLogFile("player3.txt"));
	d4 = new LogData(parseLogFile("player4.txt"));

	int highestFreq = 0;
	std::vector<std::vector<int>> positionHeatmapPositions(180, std::vector<int>(180, 0));
	std::vector<glm::vec3> positionHeatmap;
	std::vector<glm::vec3> colourHeatmap;

	for (glm::vec3 pos : d1->positionsOriginal)
	{
		int x = (int)(pos.x / 20.0f + 90.0f);
		int y = (int)(pos.y / 20.0f + 90.0f);


		positionHeatmapPositions[x][y]++;

		if (positionHeatmapPositions[x][y] > highestFreq)
			highestFreq = positionHeatmapPositions[x][y];
	}

	for (glm::vec3 pos : d2->positionsOriginal)
	{
		int x = (int)(pos.x / 20.0f + 90.0f);
		int y = (int)(pos.y / 20.0f + 90.0f);


		positionHeatmapPositions[x][y]++;

		if (positionHeatmapPositions[x][y] > highestFreq)
			highestFreq = positionHeatmapPositions[x][y];
	}

	for (glm::vec3 pos : d3->positionsOriginal)
	{
		int x = (int)(pos.x / 20.0f + 90.0f);
		int y = (int)(pos.y / 20.0f + 90.0f);


		positionHeatmapPositions[x][y]++;

		if (positionHeatmapPositions[x][y] > highestFreq)
			highestFreq = positionHeatmapPositions[x][y];
	}

	for (glm::vec3 pos : d4->positionsOriginal)
	{
		int x = (int)(pos.x / 20.0f + 90.0f);
		int y = (int)(pos.y / 20.0f + 90.0f);


		positionHeatmapPositions[x][y]++;

		if (positionHeatmapPositions[x][y] > highestFreq)
			highestFreq = positionHeatmapPositions[x][y];
	}

	for (int i = 0; i < 180; ++i)
	{
		for (int j = 0; j < 180; ++j)
		{
			if (positionHeatmapPositions[i][j] > 0)
			{
				int hits = positionHeatmapPositions[i][j];
				glm::vec3 colour(0.0f);

				if (hits > 0 && hits < 10)
					colour = glm::vec3(0.0f, 1.0f, 0.0f);
				else if (hits > 10 && hits < 20)
					colour = glm::vec3(0.0f, 1.0f, 1.0f);
				else if (hits > 20 && hits < 30)
					colour = glm::vec3(0.0f, 0.0f, 1.0f);
				else if (hits > 30 && hits < 40)
					colour = glm::vec3(1.0f, 0.0f, 1.0f);
				else if (hits > 40)
					colour = glm::vec3(1.0f, 0.0f, 0.0f);				

				positionHeatmap.push_back(glm::vec3(i - 90.0f, j - 90.0f, 0));
				colourHeatmap.push_back(glm::vec3(colour));
			}
		}
	}

	positionHeatmap = normalise(positionHeatmap, 75.0f);

	t1 = new Trajectory(d1->positions, d1->speed, glm::vec3(0.0f, 1.0f, 1.0f));
	t2 = new Trajectory(d2->positions, d2->speed, glm::vec3(0.0f, 1.0f, 0.0f));
	t3 = new Trajectory(d3->positions, d3->speed, glm::vec3(1.0f, 0.0f, 0.0f));
	t4 = new Trajectory(d4->positions, d4->speed, glm::vec3(1.0f, 1.0f, 1.0f));

	h1 = new Heatmap(d1->deaths, glm::vec3(1.0f, 0.0f, 0.0f));
	h2 = new Heatmap(d2->deaths, glm::vec3(1.0f, 0.0f, 0.0f));
	h3 = new Heatmap(d3->deaths, glm::vec3(1.0f, 0.0f, 0.0f));
	h4 = new Heatmap(d4->deaths, glm::vec3(1.0f, 0.0f, 0.0f));

	p1 = new PositionHeatmap(positionHeatmap, colourHeatmap);

	Font font(ft, "fonts/Vera.ttf");

	bool speedTraj = false;

	float zoomSpeed = 2.5f;
	float panSpeed = 0.001f;
	float zoomLevel = 1.0f;
	float playSpeed = 1.0f;

	float playStart = 0.0f;
	float playEnd = 1.0f;
	float playOffset = 0.0f;

	while (events.isAlive())
	{
		display.clear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		events.update();

		if (events.mouseState().buttons[SDL_BUTTON_LEFT])
		{
			if (player <= 1)
				player = 5;
			else
				--player;
		}

		if (events.mouseState().buttons[SDL_BUTTON_RIGHT])
		{
			if (player >= 5)
				player = 1;
			else
				++player;
		}

		if (events.mouseState().scroll != 0)
		{
				float amount = events.mouseState().scroll * zoomSpeed;

				if ((zoomLevel > 1.0f && amount < 0.0f) || (amount > 0.0f && zoomLevel < 6.25f))
				{
					camera.setPosition(camera.getPosition() - (glm::vec3(0.0f, 0.0f, 1.0f) * amount));
					zoomLevel += (amount / 10.0f);

					float speed = glm::clamp(panSpeed / zoomLevel, 0.0001f, panSpeed);
					std::cout << zoomLevel << std::endl;
				}
		}

		if (events.mouseState().heldButtons[SDL_BUTTON_MIDDLE])
		{
			float speed = glm::clamp(panSpeed / zoomLevel, 0.0001f , panSpeed);

			float xmove = events.mouseState().xrel * speed;
			float ymove = events.mouseState().yrel * speed;

			camera.setPosition(camera.getPosition() - (glm::vec3(1.0f, 0.0f, 0.0f) * xmove));
			camera.setPosition(camera.getPosition() + (glm::vec3(0.0f, 1.0f, 0.0f) * ymove));
		}

		if (events.checkKeyPress(SDL_SCANCODE_1))
		{
			changeMode(0);
		}

		if (events.checkKeyPress(SDL_SCANCODE_2))
		{
			changeMode(1);
		}

		if (events.checkKeyPress(SDL_SCANCODE_3))
		{
			changeMode(2);
		}

		if (events.checkKeyPress(SDL_SCANCODE_4))
		{
			changeMode(3);
		}

		if (events.checkKeyPress(SDL_SCANCODE_0))
		{
			speedTraj = !speedTraj;
		}

		if (events.checkKeyPress(SDL_SCANCODE_P) && mode != 2 && mode != 3)
		{
			playMode = !playMode;
		}

		if (playMode)
		{
			if(events.keyState()[SDL_SCANCODE_RETURN])
				currentPoint += (playSpeed) / 1000.0f;

			if (currentPoint > playEnd)
				currentPoint = playStart;

			if (currentPoint < playStart)
				currentPoint = playStart;


			if (events.checkKeyPress(SDL_SCANCODE_UP))
			{
				playStart = glm::clamp(playStart - 0.0005f, 0.0f, 0.5f);
				playEnd = glm::clamp(playEnd + 0.0005f, 0.5f, 1.0f);
			}

			if (events.checkKeyPress(SDL_SCANCODE_DOWN))
			{
				playStart = glm::clamp(playStart + 0.005f, 0.0f, 0.5f);
				playEnd = glm::clamp(playEnd - 0.005f, 0.5f, 1.0f);
			}

			if (events.checkKeyPress(SDL_SCANCODE_PAGEUP))
			{
				playSpeed *= 1.05f;
			}

			if (events.checkKeyPress(SDL_SCANCODE_PAGEDOWN))
			{
				playSpeed *= 0.95f;
			}

			std::string time = "Time: " + d1->time[getCurrentFrame(d1->time.size() - 1, currentPoint)];
			std::string stime = "Start: " + d1->time[getCurrentFrame(d1->time.size() - 1, playStart)];
			std::string etime = "End: " + d1->time[getCurrentFrame(d1->time.size() - 1, playEnd)];
			std::string pspeed = "Speed:" + std::to_string(playSpeed);

			font.draw("[Playing]", 0.6f, 0.9f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			font.draw(time.c_str(), 0.6f, 0.85f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			font.draw(stime.c_str(), 0.6f, 0.8f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			font.draw(etime.c_str(), 0.6f, 0.75f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			font.draw(pspeed.c_str(), 0.6f, 0.7f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		std::string titleString;
		std::string playerString = (player == 5 || mode == 3) ? "Players: All" : "Player: " + std::to_string(player);

		switch (mode)
		{
		case 0:
			// Heatmap mode
			titleString = "Mode: Death Heatmap";

			renderHeatmaps(player, camera, currentPoint);
			break;
		case 1:
			// Trajectory mode
			titleString = (speedTraj) ? "Mode: Trajectory & Velocity Map" : "Mode: Trajectory Map";
			
			renderTrajectory(player, camera, currentPoint, speedTraj);
	
			break;
		case 2:
			// Combined mode			
			titleString = (speedTraj) ? "Mode: Trajectory & Velocity Map + Death Heatmap" : "Mode: Trajectory Map & Death Heatmap";

			renderCombined(player, camera, currentPoint, speedTraj);

			break;
		case 3:
			titleString = "Mode: Position HeatMap";
			renderPositionHeatmap(camera, zoomLevel);

			break;
		}

		font.draw(titleString.c_str(), -0.9f, 0.85f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		font.draw(playerString.c_str(), -0.9f, 0.9f, 2.0f / display.width(), 2.0f / display.height(), 25, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		display.swap();
	}

	uninitialise();

	return 0;
}