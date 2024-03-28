#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;
#define Z_FAR		(500.f)
#define Z_NEAR		(.01f)
#define BUILDINGS		(100)
#define MAP_SIZE		(30)
#define MAX_XZ		(500)
/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

//Sign function
int sgn(float x)
{
    if (x < 0)
        return -1;
    else
        return 1;
}

//Interesect function for a square and circle (2d sphere and cube)

bool intersects(glm::vec3 circ, glm::vec2 rect,float r1,float r2)
{   
    const float x = max(rect.x-r2, min(circ.x, rect.x+r2));
    const float y = max(rect.y-r2, min(circ.z, rect.y+r2));

    //This is for 3d collision
    //const float z = max(rect.minZ, Math.min(sphere.z, box.maxZ));

    float distancex =pow( (circ.x - x),2);
    float distancey = pow((circ.z - y), 2);
    float cDist_sq = sqrt(distancex + distancey);

    return (cDist_sq < (r1));
}

//Intersection of two circles (2d sphere)

bool intersectsSphere(glm::vec3 circ, glm::vec3 rect, float r1, float r2)
{
    float distancex = pow((circ.x - rect.x), 2);
    float distancey = pow((circ.z - rect.z), 2);

    float cDist_sq = sqrt(distancex + distancey);

    return (cDist_sq < (r1+r2));
}

//Function from the lab to calculate the direction moved

glm::vec3 MoveForward(float distance,glm::vec3(forward))
{
    // Translates the camera using the `dir` vector computed from
    // `forward`. Movement will always keep the camera at the same
    // height. For example, if you rotate your head up/down, and then
    // walk forward, then you will still keep the same relative
    // distance (height) to the ground!
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    return  dir * distance;
}
Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    //Initializing data
 
    //The number of enemies that spawns at the start of the game
    enemyNumber = 300;

    //Camera and player's tank setup
    camera = new Camera::Camera();
    tankView = new Camera::Camera();
    tureta = new Camera::Camera();
    camera->Set(glm::vec3(0, 6, 10), glm::vec3(0,0,0), glm::vec3(0, 1, 0));
    tankView->Set(glm::vec3(0, 0, 2), glm::vec3(0,0, 0), glm::vec3(0, 1, 0));
    tureta->Set(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    fov = 40.f;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    scaleF = 0.25;
    rad = 0;
    finish = false;
    score = 0;
    //Loading the meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "senile.obj");
        meshes[mesh->GetMeshID()] = mesh;
    } {
        Mesh* mesh = new Mesh("tureta");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "tureta.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }{
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader* shader = new Shader("senile");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "senileVertex.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "senileFragment.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("tureta");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "turetaVertex.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "turetaFragment.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("corp");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "corpVertex.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "corpFragment.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("bullet");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "bulletVertex.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "bulletFragment.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //Spawning the buildings

    for (int i = 0; i < BUILDINGS; i++)
    {
        building[i].x = rand() % (2 * MAX_XZ) - MAX_XZ;
        building[i].y = rand() % (2 * MAX_XZ) - MAX_XZ;
        //Check if the builings collide with the player, if so move them
        while (intersects(tankView->position, building[i], 10, 10))
        {
            building[i].x = rand() % (2 * MAX_XZ) - MAX_XZ;
            building[i].y = rand() % (2 * MAX_XZ) - MAX_XZ;
        }
    }

    //Spawning the enemies
    for (int i = 0; i < enemyNumber; i++)
    {
        //Initializing an enemy
        enemy.push_back(Tanks(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
 
        //Checking if the enemy is spawning inside of a building

        for (int j = 0; j < BUILDINGS; j++) {
            while (intersects(enemy[i].tank->position, building[j], 5, 10))
            {
                //Giving to the enemy another random location to spawn
                enemy[i].tank->position.x = rand() % (2 * MAX_XZ) - MAX_XZ;
                enemy[i].tank->position.z = rand() % (2 * MAX_XZ) - MAX_XZ;
            }
        }
    }
}


void Tema2::FrameStart()
{   //Initlializing the clock
    begin = std::chrono::steady_clock::now();

     camera->Set(camera->position, tankView->position, glm::vec3(0, 1, 0));

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.07, 0.88, 0.88, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    //Rendering the bullets
    for (int i = 0; i < bullets.size(); i++)
    {   
        //Checking if the bullets should dissapear
        if (chrono::duration_cast<chrono::seconds>(begin - bullets[i].endT).count() > 5)
            bullets.erase(bullets.begin() + i);
        else
        {
            //Rendering the bullets
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::vec3 vec = MoveForward(100 * deltaTimeSeconds, bullets[i].forward);
            bullets[i].poz += glm::vec3(vec.x, 0, vec.z);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(bullets[i].poz.x, bullets[i].poz.y, bullets[i].poz.z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            RenderMesh(meshes["sphere"], shaders["bullet"], modelMatrix);
        }
    }
    //Tank body
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tankView->position);// glm::vec3(x, y, z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleF));
        RenderMeshTank(meshes["corp"], shaders["corp"], modelMatrix,0,glm::vec3(0.4,0.9,0.4));
    }
    //Tank turret
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, tankView->position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad2), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleF));
        RenderMesh(meshes["tureta"], shaders["tureta"], modelMatrix);
    }
    //Tank gun
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tankView->position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad2), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleF));
        RenderMesh(meshes["tun"], shaders["senile"], modelMatrix);
    }
    //Tank tracks
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tankView->position);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(rad), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleF));
        RenderMesh(meshes["senile"], shaders["senile"], modelMatrix);
    }
    //Rendering the Buildings
    for (int i = 0; i < BUILDINGS; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(building[i].x, 10, building[i].y));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(20));
       // RenderMeshTank(meshes["box"], shaders["corp"], modelMatrix,life[i], glm::vec3(0.9, 0.6, 0.1));
        RenderMeshTank(meshes["box"], shaders["corp"], modelMatrix,0, glm::vec3(0.9, 0.6, 0.1));

    }
    //Rendering the plane
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(20));
        RenderMesh(meshes["plane"], shaders["tureta"], modelMatrix);
    }

    //Rendering the mud thats at the edge of the plane
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(21));
        RenderMesh(meshes["plane"], shaders["VertexColor"], modelMatrix);
    }
    if (!finish)
        //Checking the bullets collision
    {
        for (int i = 0; i < bullets.size(); i++)
        {
            //With the buildings
            for (int j = 0; j < BUILDINGS; j++) {

                if (intersects(bullets[i].poz,building[j], 0.3, 10))
                {
                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        }
    }
    //The enemies
    for (int i = 0; i < enemy.size(); i++) {
        //Collision with the player
        if (!finish)
        {
            if (intersectsSphere(tankView->position, enemy[i].tank->position, 2, 2))
            {
                glm::vec3 Dif = (enemy[i].tank->position - tankView->position);
                float P = 4 - glm::distance(tankView->position, enemy[i].tank->position);
                glm::vec3 sal = glm::normalize(Dif) * P;
                tankView->position += sal * -0.5f;
                enemy[i].tank->position += sal * 0.5f;
                camera->position += sal * -0.5f;
            }
            //Checking if the enemy is at the edge of the plane
            if (enemy[i].radTur >= 2)
            {
                int x = enemy[i].tank->position.x;
                int z = enemy[i].tank->position.z;

                if (x > MAX_XZ || x < -MAX_XZ || z > MAX_XZ || z < -MAX_XZ)
                {
                    enemy[i].tank->TranslateForward(-2);



                    enemy[i].radTur = -1;
                    enemy[i].endT = begin - std::chrono::seconds(1);

                }
            }
            //Random movement of the enemy
            if (chrono::duration_cast<chrono::seconds>(begin - enemy[i].endT).count() > 3)
            {
                enemy[i].endT = begin;
                if (enemy[i].radTur > 0)
                    enemy[i].radTur = rand() % 50;
                else
                    enemy[i].radTur = rand() % 2;
            }
            enemy[i].Tankmove(enemy[i].radTur, deltaTimeSeconds * 50);
        }
            //Rendering the enemy
            enemy[i].modelMatrix = glm::mat4(1);
            enemy[i].modelMatrixTureta = glm::mat4(1);
            enemy[i].modelMatrix = glm::translate(enemy[i].modelMatrix, enemy[i].tank->position );
            enemy[i].modelMatrix = glm::scale(enemy[i].modelMatrix, glm::vec3(scaleF));
            enemy[i].modelMatrix = glm::rotate(enemy[i].modelMatrix,RADIANS(enemy[i].rad), glm::vec3(0, 1, 0));
            RenderMeshTank(meshes["corp"], shaders["corp"], enemy[i].modelMatrix, enemy[i].hp, glm::vec3(0.6, 0.2, 0));
            enemy[i].modelMatrixTureta = glm::translate(enemy[i].modelMatrixTureta, enemy[i].tank->position);
            enemy[i].modelMatrixTureta = glm::scale(enemy[i].modelMatrixTureta, glm::vec3(scaleF));
            enemy[i].modelMatrixTureta = glm::rotate(enemy[i].modelMatrixTureta, RADIANS(enemy[i].rad), glm::vec3(0, 1, 0));
            RenderMeshTank(meshes["tureta"], shaders["corp"], enemy[i].modelMatrixTureta, enemy[i].hp, glm::vec3(0.6, 0.3, 0));
            RenderMesh(meshes["tun"], shaders["senile"], enemy[i].modelMatrixTureta);
            RenderMesh(meshes["senile"], shaders["senile"], enemy[i].modelMatrix);
          
            if (!finish)
            {
                //Collision with the buildings
                for (int j = 0; j < BUILDINGS; j++) {
                    if (enemy[i].radTur >= 2)
                        if (intersects(enemy[i].tank->position, building[j], 2, 10))
                        {
                            enemy[i].tank->TranslateForward(-1);
                            enemy[i].radTur = -1;
                            enemy[i].endT = begin - std::chrono::seconds(1);
                        }
                }

                //Collision with another enemy
                for (int j = 0; j < enemy.size(); j++) {
                    if (j != i)
                    {
                        if (intersectsSphere(enemy[i].tank->position, enemy[j].tank->position, 2, 2))
                        {
                            glm::vec3 Dif = (enemy[j].tank->position - enemy[i].tank->position);
                            float P = 4 - glm::distance(enemy[i].tank->position, enemy[j].tank->position);
                            glm::vec3 sal = glm::normalize(Dif) * P;
                            enemy[i].tank->position += sal * -0.5f;
                            enemy[j].tank->position += sal * 0.5f;
                        }
                    }
                }

                //Collision with a bullet
                for (int j = 0; j < bullets.size(); j++)
                {
                    if (chrono::duration_cast<chrono::seconds>(begin - bullets[j].endT).count() < 4)
                        if (glm::distance(glm::vec3(enemy[i].tank->position.x, bullets[j].poz.y, enemy[i].tank->position.z), bullets[j].poz) < 2)
                        {
                            bullets.erase(bullets.begin() + j);
                            if (enemy[i].hp > 1)
                                enemy[i].hp--;
                            else
                            {
                                enemy.erase(enemy.begin() + i);
                                score++;
                            }
                            break;
                        }
                }
            }
        }
    }

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
    if (chrono::duration_cast<chrono::seconds>(begin - finishTime).count() > 60)
    {
        if (!finish)
        {
            cout << "Congratulations!\n this is your score: ";
        cout << score;
        cout << "\n";
    }
        finish = true;
       
    }
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    mesh->Render();
}
void Tema2::RenderMeshTank(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,int hp,const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    //Adding the tank's hp to the shader
    int da = glGetUniformLocation(shader->program, "hp");
    glUniform1f(da, hp);

    //Adding the tank's color to the shader
    int cul = glGetUniformLocation(shader->program, "color");
    glUniform3f(cul, color.r,color.g,color.b);
    mesh->Render();
}
/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{   
    if (!finish)
    {
        //Rotate the tank to the left
        if (window->KeyHold(GLFW_KEY_A)) {
            rad += 55 * deltaTime;
            tankView->RotateFirstPerson_OY(RADIANS(55 * deltaTime));
            camera->RotateThirdPerson_OY(RADIANS(55 * deltaTime));
            tureta->RotateFirstPerson_OY(RADIANS(55 * deltaTime));
        }
        //Move the tank forward
        if (window->KeyHold(GLFW_KEY_W)) {
            tankView->TranslateForward(deltaTime * 25);
            camera->MoveForward(deltaTime * 25);

            //Check the collision with the buildings
            for (int j = 0; j < BUILDINGS; j++) {
                if (intersects(tankView->position,building[j], 2, 10))
                {
                    tankView->TranslateForward(-1);
                    camera->MoveForward(-1);
                }
            }
        }

        //Move the tank backwoards
        if (window->KeyHold(GLFW_KEY_S)) {
            tankView->TranslateForward(-deltaTime * 25);
            camera->MoveForward(-deltaTime * 25);

            //Check the collision with the buildings
            for (int j = 0; j < BUILDINGS; j++) {
                if (intersects(tankView->position,building[j], 2, 10))
                {
                    tankView->TranslateForward(1);
                    camera->MoveForward(1);
                }
            }
        }

        //Rotate the tank to the right
        if (window->KeyHold(GLFW_KEY_D)) {
            rad -= 55 * deltaTime;
            tureta->RotateFirstPerson_OY(RADIANS(-55 * deltaTime));
            tankView->RotateFirstPerson_OY(RADIANS(-55 * deltaTime));
            camera->RotateThirdPerson_OY(RADIANS(-55 * deltaTime));
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    
    
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
                  //Checking for the camera to dont move in the front of the tank
            if (rotx > (-0.6) || deltaY < 0)
                if (rotx < (0.5) || deltaY > 0)
                {
                    //Rptate the camera
                    rotx -= sensivityOY * deltaY;
                    camera->RotateThirdPerson_OX(sensivityOY * -deltaY);

                }
            //Rotate the turret
             rad2 -= 100 * sensivityOY * deltaX;            
            tureta->RotateFirstPerson_OY(-RADIANS(100 * sensivityOY * deltaX));

        
    

}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    
    if(!finish)
    if (button==GLFW_MOUSE_BUTTON_2) {
        if (chrono::duration_cast<chrono::seconds>(begin - endMyTank).count() > 1)
        {
            endMyTank = begin;
        //    glm::vec3 vec = MoveForward( 0.5, tureta->forward);
            bullets.push_back(Bullet(glm::vec3(tankView->position.x  , tankView->position.y + 1.7, tankView->position.z ), tureta->forward));

            //Add enemies if their number is under a certain level
            if (enemy.size() < 100)
            {
                enemy.push_back(Tanks(glm::vec3(rand() % (2 * MAX_XZ) - MAX_XZ, 0, rand() % (2 * MAX_XZ) - MAX_XZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
                for (int j = 0; j < BUILDINGS; j++) {
                    //Dont spawn enemies inside the buildings
                    while (intersects(enemy[enemy.size() - 1].tank->position,building[j], 5, 10))
                    {
                        enemy[enemy.size() - 1].tank->position.x = rand() % (2 * MAX_XZ) - MAX_XZ;
                        enemy[enemy.size() - 1].tank->position.z = rand() % (2 * MAX_XZ) - MAX_XZ;
                    }
                }
            }
        }
    }
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods){
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
