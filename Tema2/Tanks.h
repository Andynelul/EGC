#ifndef Tanks_H

#define Tanks_H

#include <string>
#include <chrono>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/Tema2/homework_camera.h"
#define MAX_XZ 500
//Class for the enemies
class Tanks {
public:
	glm::vec3 poz;
	glm::vec3 center;
	glm::vec3 up;
	float radTur;
	float rad=0;
	float scale = 0.5;
	int hp=3;
	float dir=0.2;
	float dirX = 0.2;
	std::chrono::steady_clock::time_point endT;
	glm::mat4 modelMatrix;
	glm::mat4 modelMatrixTureta;
	Camera::Camera* tank;
	Camera::Camera* tureta;
	glm::vec3 forw = glm::vec3(0);

	Tanks(glm::vec3 poz,
		glm::vec3 center,
		glm::vec3 up
	) {
		poz = glm::vec3(0, 0, rand() % 250 - 125);
		tureta = new Camera::Camera();
		tank = new Camera::Camera();
		modelMatrix = glm::mat4(1);
		modelMatrixTureta = glm::mat4(1);
		this->rad = 0;
		radTur = 0;
		tank->Set( (glm::vec3(abs(poz.x),poz.y,abs(poz.z))),
			 center,
			 up
		);
		tureta->Set(poz,
			center,
			up
		);
		tank->position = glm::vec3(rand() % (2 * MAX_XZ) - MAX_XZ, 0, rand() % (2 * MAX_XZ) - MAX_XZ);
		std::chrono::steady_clock::time_point endT = std::chrono::steady_clock::now();

	}
	void Tankmove(int i, float delta)
	{
			if (i == -1)
			{
				this->tank->TranslateForward(-delta / 15);
			}
			if (i == 0)
			{
				this->tank->RotateFirstPerson_OY(RADIANS(delta));
				this->rad += delta;
			}
			else if (i == 1)
			{
				this->tank->RotateFirstPerson_OY(RADIANS(-delta));
				this->rad -= delta;
			}
			else if (i >= 2)
			{
				this->tank->TranslateForward(delta / 3);
			}
	}
};
#endif