#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/homework_camera.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/Tanks.h"
#include "lab_m1/Tema2/Bullet.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderMeshTank(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int hp, const glm::vec3& color);

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        Camera::Camera *camera;
        Camera::Camera* tankView;
        Camera::Camera* tureta;
        glm::mat4 projectionMatrix;
        bool projectionType;
        GLfloat fov;
        float scaleF;
        float rad,rad2=0;
        glm::vec3 forw = glm::vec3(0);
        std:: vector<Tanks> enemy;
        int enemyNumber;
        glm::vec2 building[100];
        std::vector<Bullet> bullets;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point endMyTank = std::chrono::steady_clock::now();
        float rotx=0;
        std::chrono::steady_clock::time_point finishTime = std::chrono::steady_clock::now();
        bool finish;
        int score;
    };
}   // namespace m1
