#include "spawn.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "spawn.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"


//Star constructor 

Star::Star(glm::vec3 poz,
	glm::vec3 color,
	bool active,
	int R) :poz(poz), color(color), active(active), R(R)
{
	modelMatrix = glm::mat3(1);
	mesh = object2D::CreateStar("star", glm::vec3(0, 0, 4), R, color, true);
	modelMatrix *= transform2D::Translate(poz.x, poz.y);
}

//Star moving method

void Star::move(float dist) {
	poz.x += 150 * dist;
	modelMatrix = glm::mat3(1);
	rotate -= dist * 150;

	modelMatrix *= transform2D::Translate(poz.x, poz.y);
	modelMatrix *= transform2D::Rotate(RADIANS(rotate));
}

//Constructor for the points

StarPoint::StarPoint(glm::vec3 poz,
	glm::vec3 color,
	int R) :poz(poz), color(color), R(R)
{
	modelMatrix = glm::mat3(1);
	mesh = object2D::CreateStar("star", glm::vec3(0, 0, 5), R, color, true);
	modelMatrix *= transform2D::Translate(poz.x, poz.y);
}

//Method for mouse over pointStars

bool StarPoint::localize(int x, int y)
{
	if (glm::distance(poz, glm::vec3(x, y, 5)) <= 4 * R)
	{
		return true;

	}
	else return false;
}

//Moving method for tanks (diamonds)

int Tank::move(int dist)
{
	return poz.x += dist;
}

//Constructor method for the tanks

Tank::Tank(glm::vec3 poz,
	glm::vec3 color,
	bool active,
	int length,
	int height) :poz(poz), color(color), active(active), length(length), height(height)
{
	meshDiamond = object2D::CreateDiamond("diamond", glm::vec3(0, 0, poz.z), length, height, color, true);
	meshTank = object2D::CreateTank("tank", glm::vec3(0, 0, poz.z), length, height, color, true);
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(poz.x, poz.y);
}

//Method called when tanks are dissapearing

void Tank::dis(float d) {
	if (scaleX >= 0) {
		scaleX -= d;
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(poz.x, poz.y);
		modelMatrix *= transform2D::Scale(scaleX, scaleX);
		active = false;
	}
}

int Hexagon::move(int dist)
{
	return poz.x += dist;
}

Hexagon::Hexagon(glm::vec3 poz,
	glm::vec3 colorIn,
	glm::vec3 colorOut,
	bool active,
	int R) :poz(poz), colorIn(colorIn), colorOut(colorOut), active(active), R(R)
{
	modelMatrix = glm::mat3(1);
	meshOut = object2D::CreatePolygon("polygon", glm::vec3(0, 0, 2), R, colorOut);
	meshIn = object2D::CreatePolygon("polygon", glm::vec3(0, 0, 3), R / 2, colorIn);
	modelMatrix *= transform2D::Translate(poz.x, poz.y);
	scaleX = 0.9;
}

void Hexagon::move(float dist) {
	poz.x -= 50 * dist;
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(poz.x, poz.y);

}

void Hexagon::dis(float d) {
	if (scaleX >= 0) {
		scaleX -= d;
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(poz.x, poz.y);
		modelMatrix *= transform2D::Scale(scaleX, scaleX);
		active = false;
	}


}