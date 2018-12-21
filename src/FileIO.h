#ifndef _FILEIO_H
#define _FILEIO_H

#include <vector>
#include <string>
#include <map>
#include <GLM\glm.hpp>

struct LogData
{
	std::vector<std::string> time;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> positionsOriginal;
	std::vector<glm::vec3> velocity;
	std::vector<int> health;
	std::vector<glm::vec3> deaths;
	std::vector<float> speed;

	float largestX;
	float largestY;
};

const std::vector<std::string> readFileLine(std::string fileName);
const std::string readFile(std::string fileName);
const std::vector<std::string> splitString(std::string input, char delim);
LogData parseLogFile(std::string filename);

std::vector<glm::vec3> normalise(std::vector<glm::vec3> input, float max);

#endif