#include <string>
#include <chrono>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class Bullet {
public:
	glm::vec3 poz;
	Bullet(glm::vec3 pozi, glm::vec3 forwardi)

	{
		poz = pozi;
		forward = forwardi;
		endT = std::chrono::steady_clock::now();
	}
};