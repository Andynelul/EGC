#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/spawn.h"
#include <chrono>

namespace m1
{

    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat3 modelMatrix;
        int rotation;
        int NrStar=10, NrHexa=10,nrHexaOld=10,nrStarOld=10;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point endP = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point starTime = std::chrono::steady_clock::now();
        int poz[3] = { 75,260,445 };
        glm::vec3 colors[4] = { glm::vec3(1,0,0),glm::vec3(0,0,1),glm::vec3(1,1,0),glm::vec3(1,0,1)};
        int pozX[3] = { 150,345,520 };
        int pozGUI[4] = { 75,225,375,525 };
        Hexagon *pentagon;
        Tank *tank,*tankGUI;
        Star *stea;
        int lives,stars,starPointNr;
        std::vector<StarPoint> starPoint;
        int cost;
        bool selected;
    };
}   
