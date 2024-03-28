#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <chrono>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/spawn.h"
#include "lab_m1/Tema1/interactions.h"


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 130;
    rotation = 0;
    lives = 3;
    stars = 0;

    //Clock
    begin = std::chrono::steady_clock::now();
    endP = std::chrono::steady_clock::now();
    starPointNr = 2;
    cost = 999;


    //Meshes

    //Tank holders
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);

    //Mini tank holders
    Mesh* square1m = object2D::CreateSquare("square1m", corner, squareSide/10, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1m);

    //Mesh lives
    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 0, 0),true);
    AddMeshToList(square2);

    //Mesh GUI holders
    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    //Mini GUI holders
    Mesh* square3m = object2D::CreateSquare("square3m", corner, squareSide/10, glm::vec3(0, 0, 1));
    AddMeshToList(square3m);

    //Mesh prices GUI and total stars owned by player
    Mesh* stars = object2D::CreateStar("stars", corner, 5, glm::vec3(4, 4, 4),true);
    AddMeshToList(stars);

    //Red stars for the ability *When the stars have negative value*
    Mesh* starsN = object2D::CreateStar("starsN", corner, 5, glm::vec3(1, 0, 0), true);
    AddMeshToList(starsN);

    //Endline mesh
    Mesh* square4 = object2D::CreateRectangle("square4", corner, 50,500, glm::vec3(1, 0, 0),true);
    AddMeshToList(square4);

    //Endline mini mesh
    Mesh* square4m = object2D::CreateRectangle("square4m", corner, 5, 50, glm::vec3(1, 0, 0), true);
    AddMeshToList(square4m);

    //Initial memory allocation for tank, projectiles,GUI and enemies  
    stea = (Star*)calloc(NrStar, sizeof(Star));
    tank = (Tank*)calloc(9, sizeof(Tank));
    tankGUI = (Tank*)calloc(5, sizeof(Tank));

    for (int i = 0; i < starPointNr; i++)
    {
        starPoint.push_back(StarPoint(glm::vec3(rand() % 1280, rand() % 720, 0), glm::vec3(1, 1, 1), 10));
        starPointNr = 2;
    }

    pentagon = (Hexagon*)calloc(NrHexa, sizeof(Hexagon));
     for (int i = 0; i < 3; i++)
         for (int j = 0; j < 3; j++)
         {
             tank[rotation] = Tank(glm::vec3(pozX[i % 3], poz[j % 3], 3), colors[rand() % 3], false, 0, 0);
             tank[rotation]. endT = std::chrono::steady_clock::now();
       rotation++;
         }
     for (int j = 0; j < 5; j++)
     {
         tankGUI[j] = Tank(glm::vec3(pozGUI[j % 4], 645, 2), colors[j], true, 50, 50);
     }
     rotation = 0;
}


void Tema1::FrameStart()
{
  //Updating clock at every start of the 
    begin = std::chrono::steady_clock::now();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  int   c = rand() % 20;

  //Counter time and deploy star points
  if (chrono::duration_cast<chrono::seconds>(begin - starTime).count() > 5)
  {
      starTime = std::chrono::steady_clock::now();
      for (int i = 0; i < 5; i++)
      {
          starPoint.push_back(StarPoint(glm::vec3(rand() % 1280, rand() % 720, 0), glm::vec3(1, 1, 1), 10));
          starPointNr ++;
      }
  }

  //Counter time and deploy enemies
  if (chrono::duration_cast<chrono::seconds>( begin-endP).count() > 3)
  {
      endP = std::chrono::steady_clock::now();
      for (int c=0;c<NrHexa;c++)
      if(!pentagon[c].active){
       

          pentagon[c].active = true;
          pentagon[c].poz.x = 1400;
          pentagon[c].poz.y = poz[rand() % 3];
          pentagon[c].poz.z = 3;
          pentagon[c].colorIn = glm::vec3(0, 1, 1);
          pentagon[c].colorOut = colors[rand() % 4];
          pentagon[c].R = 50;
          pentagon[c].scaleX = 0.9;
          pentagon[c].modelMatrix *= transform2D::Translate(pentagon[c].poz.x, pentagon[c].poz.y);
          pentagon[c].life = 3;

          //Cahnce of a boss with 20HP
          if (rand() % 100 == 0)
          {
              pentagon[c].life = 20;
              pentagon[c].colorIn = glm::vec3(0, 0, 0);
          }
          pentagon[c].meshIn = object2D::CreatePolygon("polygon", glm::vec3(0, 0, 4), pentagon[c].R / 2, pentagon[c].colorIn);
          pentagon[c].meshOut = object2D::CreatePolygon("polygon", glm::vec3(0, 0, 3), pentagon[c].R, pentagon[c].colorOut);
          //Check if the enemies need more memory
          if (c +2 >= NrHexa)
              
              {
                  nrHexaOld = NrHexa;
                  NrHexa += 10;
                  Hexagon* penta=(Hexagon*)realloc(pentagon, NrHexa * sizeof(Hexagon));
                  if (penta != NULL)
                      pentagon = penta;
                  for (int i = nrHexaOld; i < NrHexa; i++)
                      pentagon[i] = Hexagon(glm::vec3(0), glm::vec3(0), glm::vec3(0), false, 0);
              }

          //exit after u deploy a enemy
          c = NrHexa;
      }
     
  }
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    //If the mouse button is pressed render the selected item from the GUI
    if (selected == true)
    {
        RenderMesh2D(tankGUI[4].meshDiamond, shaders["VertexColor"], tankGUI[4].modelMatrix);
        RenderMesh2D(tankGUI[4].meshTank, shaders["VertexColor"], tankGUI[4].modelMatrix);
    }

    //If there are star points on the screen render them
    for (int i = 0; i < starPointNr; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(starPoint[i].poz.x, starPoint[i].poz.y);
        RenderMesh2D(starPoint[i].mesh, shaders["VertexColor"], modelMatrix);
    }

    //If the user has stars render them
    for (int i = 0; i < stars; i++)
    {
        int sala = i % 14;
        int sal2 = i / 14;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(840 + sala * 30, 565 - 30*sal2);
        RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    }

    //If the number is negative render them with red
    if (stars < 0)
        for(int i=0;i>stars;i--)
    {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(840 + abs(i)* 30, 565 - 30 );
            //modelMatrix *= transform2D::Rotate(R);
            RenderMesh2D(meshes["starsN"], shaders["VertexColor"], modelMatrix);
    }
    // first row of the map
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(85, 10);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(85, 195);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
   
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(85, 380);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    //second row
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 10);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 195);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 380);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    //third row
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(455, 10);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(455, 195);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(455, 380);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    //end
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(10, 10);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);
    
    //GUI
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(10, 580);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(10, 550 );
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);

    //Render the tanks
    for (int j = 0; j < 4; j++)
    {
        RenderMesh2D(tankGUI[j].meshDiamond, shaders["VertexColor"], tankGUI[j].modelMatrix);
        RenderMesh2D(tankGUI[j].meshTank, shaders["VertexColor"], tankGUI[j].modelMatrix);
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(160, 580);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(160, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(190, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(310, 580);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(310, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(340, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(460, 580);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(490, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(520, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(550, 550);
    RenderMesh2D(meshes["stars"], shaders["VertexColor"], modelMatrix);

    // Ability mesh
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 580);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(608, 581);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(608, 600);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(608, 619);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    //second row
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(627, 581);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(627, 600);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(627, 619);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    //third row
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(646, 581);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(646, 600);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(646, 619);
    RenderMesh2D(meshes["square1m"], shaders["VertexColor"], modelMatrix);

    //end
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 581);
    RenderMesh2D(meshes["square4m"], shaders["VertexColor"], modelMatrix);
    //End of ability mesh

    //Lives counter
    if (lives >= 3) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(840, 580);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }
    if (lives >= 2)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(990, 580);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }
    if (lives >= 1)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1140, 580);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }
    else{
        window->Close();
    }

    //Render the projejctiles
    for (int i = 0; i < NrStar; i++)
    {
        if (stea[i].active)
        {
            stea[i].move(deltaTimeSeconds);
            RenderMesh2D(stea[i].mesh, shaders["VertexColor"], stea[i].modelMatrix);
            if (stea[i].poz.x > window->GetResolution().x + 20)
            {
                
                stea[i].active = false;
            }
        }
    }

    //Render the tanks
    for (int i = 0; i < 9; i++)
        if(tank[i].active==true)
 {   RenderMesh2D(tank[i].meshDiamond, shaders["VertexColor"], tank[i].modelMatrix);
    RenderMesh2D(tank[i].meshTank, shaders["VertexColor"], tank[i].modelMatrix);
}

    //Render the enemies
for (int i = 0; i < NrHexa; i++)
if (pentagon[i].active)
{   //Check if an enemy is at the end
    if (pentagon[i].poz.x > 25)
    {

        pentagon[i].move(deltaTimeSeconds);
        RenderMesh2D(pentagon[i].meshOut, shaders["VertexColor"], pentagon[i].modelMatrix);
        RenderMesh2D(pentagon[i].meshIn, shaders["VertexColor"], pentagon[i].modelMatrix);
    }
    else
    {
        lives--;
        pentagon[i].active = false;
    }
 
}


}


void Tema1::FrameEnd()
{
    //Check enemy-projectile collision
    for (int i = 0; i < NrHexa; i++)
        for (int j = 0; j < NrStar; j++)
            if (pentagon[i].active && stea[j].active) {
                if(pentagon[i].colorOut==stea[j].color)
                if (collision(stea[j].poz, 2 * stea[j].R, pentagon[i].poz, pentagon[i].R))
                {
                        
                        stea[j].active = false;
                    if (pentagon[i].life == 1)
                        pentagon[i].active = false;
                    else
                    {
                        pentagon[i].life--;
                        //Chance of a mutation for a pentagon when a life is drained
                        if (pentagon[i].colorIn ==glm::vec3(0,1,1 ))
                        {
                            if (stars > 15)
                                if (stars < 65)
                                {
                                    if (rand() % (70 - stars) == 0)
                                    {
                                        pentagon[i].life = 20;
                                        pentagon[i].poz.x = 1400;
                                        pentagon[i].colorIn = glm::vec3(0, 0, 0);
                                        pentagon[i].meshIn = object2D::CreatePolygon("pentagon", glm::vec3(0, 0, 4), pentagon[i].R / 2, pentagon[i].colorIn);
                                    }
                                }
                                    else if (pentagon[i].life < 5&& stars>=65)
                                    {

                                        pentagon[i].life = 5;
                                        pentagon[i].poz.x = 1400;
                                        pentagon[i].colorIn = glm::vec3(255, 0.3, 0);
                                        pentagon[i].meshIn = object2D::CreatePolygon("pentagon", glm::vec3(0, 0, 4), pentagon[i].R / 2, pentagon[i].colorIn);
                                    }
                        }
                        if (rand() % 50 == 1)
                            pentagon[i].poz.y = poz[rand() % 3];
                        if (rand() % 50 == 1)
                        {
                            pentagon[i].poz.y = poz[rand() % 3];
                            pentagon[i].colorOut = colors[rand() % 3];
                            pentagon[i].meshOut = object2D::CreatePolygon("pentagon", glm::vec3(0, 0, 3), pentagon[i].R, pentagon[i].colorOut);
                        }
                    }

                }
            }
    for (int i = 0; i < NrHexa; i++)
        for (int j = 0; j < 9; j++)
            if (pentagon[i].active && tank[j].active) {
                if (pentagon[i].poz.y == tank[j].poz.y ){
                    //Check tank-enemy collision
                    if (collision(tank[j].poz, tank[j].length, pentagon[i].poz, pentagon[i].R))
                    {
                        tank[j].active = false;  

                    }
                    else if(tank[j].color==pentagon[i].colorOut)
                        //Check time from the last projectile launched 
                     if (chrono::duration_cast<chrono::seconds>(begin - tank[j].endT).count() > 1) {
                        for (int k = 0; k < NrStar; k++)
                        {   
                            tank[j].endT= std::chrono::steady_clock::now();
                            if (!stea[k].active)
                            {
                                stea[k].active = true;
                                stea[k].color = tank[j].color;
                                stea[k].poz.x = tank[j].poz.x;
                                stea[k].poz.y = tank[j].poz.y;
                                stea[k].R = 10;
                                stea[k].mesh = object2D::CreateStar("star", glm::vec3(0, 0, 4), stea[k].R, stea[k].color, true);
                                modelMatrix *= transform2D::Translate(stea[k].poz.x, stea[k].poz.y);
                                //Check if the projectiles need more memory
                                if (k + 2 >= NrStar)
                                    {
                                    
                                        nrStarOld = NrStar;
                                        NrStar += 10;
                                        Star * stea2 =(Star*)realloc(stea, sizeof(Star) * NrStar);
                                        if (stea2 != NULL)
                                            stea = stea2;
                                        for (int i = nrStarOld; i < NrStar; i++)
                                            stea[i] = Star(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), false, 0);
                                    }
                                k = NrStar;
                            }
                        }
                    }
            }
            }
    //If the enemy is inactive make it dissapear
    for (int i = 0; i < NrHexa; i++)
    if (!pentagon[i].active)
    {
        pentagon[i].dis(0.01);
        RenderMesh2D(pentagon[i].meshIn, shaders["VertexColor"], pentagon[i].modelMatrix);
        RenderMesh2D(pentagon[i].meshOut, shaders["VertexColor"], pentagon[i].modelMatrix);
        if (pentagon[i].scaleX < 0.1)
            pentagon[i].poz.x = 5000;

    }
    //If the tank is inactive make it dissapear
    for (int i = 0; i < 9; i++)
        if (!tank[i].active)
        {
            tank[i].dis(0.01);
            RenderMesh2D(tank[i].meshDiamond, shaders["VertexColor"], tank[i].modelMatrix);
            RenderMesh2D(tank[i].meshTank, shaders["VertexColor"], tank[i].modelMatrix);
        }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
   
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //Update the position of the GUI tank at the mouse coordinates
    if (selected == true)
    {
        tankGUI[4].modelMatrix = glm::mat3(1);
        tankGUI[4].modelMatrix *= transform2D::Translate(mouseX, window->GetResolution().y - mouseY);
    }
   
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2)
    {   //Check if the player presses on a star point
        for (int i = 0; i < starPointNr; i++)
        {

            if (starPoint[i].localize(mouseX, window->GetResolution().y - mouseY))
            {
                starPoint.erase(starPoint.begin() + i);
                starPointNr--;
                stars++;
                break;
            }
        }
        //Check if the player presses on the GUI and where
        int tankpoz = localizeGUI(mouseX, window->GetResolution().y - mouseY);
        if (tankpoz != -1)
        {
            if (tankpoz >= 2)
                cost = tankpoz;
            else
                cost = tankpoz + 1;
            if (cost <= stars)
            {
                selected = true;
                tankGUI[4].color = tankGUI[tankpoz].color;
                tankGUI[4].poz = tankGUI[tankpoz].poz-glm::vec3(0,0,1);
                tankGUI[4].active = false;
                tankGUI[4].height = tankGUI[tankpoz].height;
                tankGUI[4].length = tankGUI[tankpoz].length;
                tankGUI[4].meshDiamond = object2D::CreateDiamond("diamond", glm::vec3(0, 0, 1), tankGUI[4].length, tankGUI[4].height, tankGUI[4].color, true);

                tankGUI[4].meshTank = object2D::CreateTank("tank", glm::vec3(0, 0, 1), tankGUI[4].length, tankGUI[4].height, tankGUI[4].color, true);
                tankGUI[4].modelMatrix = glm::mat3(1);
                tankGUI[4].modelMatrix *= transform2D::Translate(tankGUI[4].poz.x, tankGUI[4].poz.y);
            }
        }
        //Check if the player presses on the ability button 
        
        
 
        
    }
    if (button == GLFW_MOUSE_BUTTON_3)
    {
        //Check if the player presses the right mouse on  an active tank and deletes it
        int pozx;
        int pozy;
        int tankpoz2 = localizePoz(mouseX, window->GetResolution().y - mouseY, &pozx, &pozy);
        if (tankpoz2 != -1)
        {
            tank[3 * pozx + pozy].active = false;
        }
        if (localizePower(mouseX, window->GetResolution().y - mouseY) == 1)
        {
            if (stars >= 10)
            {
                for (int i = 0; i < NrHexa; i++)
                {
                    pentagon[i].active = false;
                }
                for (int i = 0; i < 9; i++)
                {
                    tank[i].active = false;
                }
                starPoint.clear();
                starPointNr = 0;

                stars = -10;
            }
        }
    }

    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
       //Check if the player releases the tank where he should, if he has money to buy it and puts it down
    if (button == GLFW_MOUSE_BUTTON_2)
    if(selected)
    if (cost <= stars)
    {
        int pozx;
        int pozy;
        int tankpoz2 = localizePoz(mouseX, window->GetResolution().y - mouseY, &pozx, &pozy);
        if (tankpoz2 != -1)
            if(!tank[3 * pozx + pozy].active)
        {
            stars -= cost;
            int poz = 3 * pozx + pozy;
            tank[3 * pozx + pozy].color = tankGUI[4].color;
            tank[3 * pozx + pozy].active = true;
            tank[poz].length = tankGUI[4].length;
            tank[poz].height = tankGUI[4].height;
            tank[poz].meshDiamond = object2D::CreateDiamond("diamond", glm::vec3(0, 0, 3), tank[poz].length, tank[poz].height, tank[poz].color, true);
            tank[poz].meshTank = object2D::CreateTank("tank", glm::vec3(0, 0, 3), tank[poz].length, tank[poz].height, tank[poz].color, true);
            tank[poz].scaleX = 1;
            tank[poz].modelMatrix = glm::mat3(1);
            tank[poz].modelMatrix *= transform2D::Translate(tank[poz].poz.x, tank[poz].poz.y);
            cost = 999;
        }
        selected = false;
    }
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
