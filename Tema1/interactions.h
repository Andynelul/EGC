#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "spawn.h"

bool collision(glm::vec3 c1, int r1, glm::vec3 c2, int r2);
int localizeGUI(int x, int y);
int localizePoz(int x, int y, int* pozx, int* pozy);
int localizePower(int x, int y);