#include "FileIO.h"

#include <iostream>
#include <fstream>
#include <sstream>

const std::vector<std::string> readFileLines(std::string fileName)
{
	std::string line;
	std::vector<std::string> lines = std::vector<std::string>();

	// Open file for reading
	std::ifstream file;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		while (getline(file, line))
		{
			lines.push_back(line);
		}

		file.close();
	}
	else
	{
		std::cerr << "Could not open file: " << fileName << std::endl;
	}

	return lines;
}

const std::string readFile(std::string fileName)
{
	std::vector<std::string> lines = readFileLines(fileName);
	std::string file = "";

	for (std::string line : lines)
	{
		file.append(line + "\n");
	}

	return file;
}

const std::vector<std::string> splitString(std::string input, char delim)
{
	std::vector<std::string> parts;
	std::string token;
	std::stringstream inputStream(input);

	while (getline(inputStream, token, delim))
	{
		parts.push_back(token);
	}

	return parts;
}

LogData parseLogFile(std::string filename)
{
	LogData data;

	std::vector<std::string> lines = readFileLines(filename);

	std::cout << "Read file ok" << std::endl;

	int i = 0;

	// Loop through file lines and parse data.
	for (std::string line : lines)
	{
		++i;

		std::vector<std::string> split = splitString(line, ' ');

		if (split[2] == "HEALTH:")
		{
			data.health.push_back(atoi(split[4].c_str()));
		}
		else if (split[2] == "POS:")
		{
			data.positions.push_back(glm::vec3(std::stof(split[4]), std::stof(split[5]), std::stof(split[6])));
		}
		else if (split[2] == "VEL:")
		{
			data.velocity.push_back(glm::vec3(std::stof(split[4]), std::stof(split[5]), std::stof(split[6])));
		}

		if (i == 3)
		{
			data.time.push_back(split[0]);
			i = 0;
		}
	}

	// Normalised data
	data.positionsOriginal = data.positions;
	data.positions = normalise(data.positions, 1500.0f);
	data.velocity = normalise(data.velocity, 1500.0f);

	// Extrapolate new data from exisiting data
	// deaths can be found when health <= 0
	std::string lastDeathTime = "";

	for (unsigned int i = 0; i < data.health.size(); ++i)
	{
		if (data.health[i] <= 0 && lastDeathTime != data.time[i])
		{
			lastDeathTime = data.time[i];
			data.deaths.push_back(data.positions[i]);
		}
	}
	
	// speed can be calculated from 3D velocity
	for (int i = 0; i < data.velocity.size(); ++i)
	{
		if (data.health[i] <= 0)
		{
			data.speed.push_back(0.0f);
		}
		else
		{
			data.speed.push_back(glm::length(data.velocity[i]));
		}
	}

	return data;
}

std::vector<glm::vec3> normalise(std::vector<glm::vec3> input, float max)
{
	std::vector<glm::vec3> normalised;

	for (glm::vec3 v : input)
	{
		glm::vec3 temp(v.x / max, v.y / max, v.z / max);

		normalised.push_back(temp - 0.25f);
	}

	return normalised;
}

std::vector<float> normalise(std::vector<float> input)
{
	std::vector<float> normalised;
	float largest = 0.0f;

	for (float f : input)
	{
		if (f > largest)
			largest = f;
	}

	for (float f : input)
	{
		normalised.push_back(f / largest);
	}

	return normalised;
}