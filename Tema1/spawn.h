#pragma once

#include <string>
#include <chrono>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

//A class for projectiles 
class Star {
private:
	int rotate=0;
public:
	glm::vec3 poz;
	glm::vec3 color;
	bool active;
	int R;
	Mesh* mesh;
	glm::mat3 modelMatrix;

	void move(float dist);

		Star(glm::vec3 poz,
			glm::vec3 color,
			bool active,
			int R);
};

//Class for the points
class StarPoint {
private:
	int rotate = 0;
public:
	glm::vec3 poz;
	glm::vec3 color;
	int R;
	Mesh* mesh;
	glm::mat3 modelMatrix;

	StarPoint(glm::vec3 poz,
		glm::vec3 color,
		int R);

	bool localize(int x, int y);
};

//Class for the tanks
class Tank {
public:
	glm::vec3 poz;
	glm::vec3 color;
	bool active;
	int length;
	int height;
	Mesh* meshDiamond;
	Mesh* meshTank;
	std::chrono::steady_clock::time_point endT;
	glm::mat3 modelMatrix;
	float scaleX=1;

	int move(int dist);

	Tank(glm::vec3 poz,
		glm::vec3 color,
		bool active,
		int length,
		int height);

	void dis(float d);
};

//Enemy class
class Hexagon {
public:
	float scaleX ;
	glm::vec3 poz;
	glm::vec3 colorOut;
	glm::vec3 colorIn;
	bool active;
	int R;
	int life = 3;
	Mesh* meshOut;
	Mesh* meshIn;
	glm::mat3 modelMatrix;

	int move(int dist);

	Hexagon(glm::vec3 poz,
		glm::vec3 colorIn,
		glm::vec3 colorOut,
		bool active,
		int R);

	void move(float dist);

	void dis(float d);
};