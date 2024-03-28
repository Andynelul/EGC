#include "spawn.h"
#include "interactions.h"
#include <vector>
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "spawn.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

//Check for colision
bool collision(glm::vec3 c1,int r1, glm::vec3 c2,int r2)
{
	if (c1.y != c2.y)
		return false;
	else if (glm::distance(c1.x, c2.x) <= r1 + r2)
	{
		return true;
	}
	else return false;
	
}
//Check if the mouse is over the GUI  
int localizeGUI(int x,int y)
{
	int pozX[4] = {75,225,375,525};
	int pozY = 645;
	for(int i=0;i<4;i++)
	if(glm::distance(glm::vec3(x,y,1),glm::vec3(pozX[i],pozY,1))<65)
	{
		return i;
	}
	return -1;

}

//Check if the mouse is over a tank spot
int localizePoz(int x, int y,int *pozx,int *pozy)
{
	int poz[3] = { 75,260,445 };
	glm::vec3 colors[4] = { glm::vec3(1,0,0),glm::vec3(0,0,1),glm::vec3(1,1,0),glm::vec3(1,0,1) };
	int pozX[3] = { 150,345,520 };
	for (int i = 0; i < 3; i++)
		for(int j=0;j<3;j++)
		if (glm::distance(glm::vec3(x, y, 1), glm::vec3(pozX[i], poz[j], 1))<65)
		{	
			pozx[0] = i;
			pozy[0] = j;
			return i;
		}
	return -1;

}

//Check if the mouse is over the ability
int localizePower(int x, int y)
{
			if (glm::distance(glm::vec3(x, y, 1), glm::vec3(665, 645, 1)) < 65)
			{
				
				return 1;
			}
	return -1;
}