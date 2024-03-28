#include <string>
#include <chrono>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/Tema2/homework_camera.h"

//Class for a bullet

class Bullet {
public:
	glm::vec3 poz;
	glm::vec3 forward;
	std::chrono::steady_clock::time_point endT;
	Bullet(glm::vec3 pozi, glm::vec3 forwardi)

	{
		poz = pozi;
		forward = forwardi;
		endT = std::chrono::steady_clock::now();
	}
};