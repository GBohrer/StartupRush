#include <raylib.h>

#include "manager.hpp"

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "STARTUP RUSH");
    SetTargetFPS(60);

    Manager& manager = Manager::GetManager();
    manager.Init();

    manager.CreateStartupSamples(4);

    while (manager.isRunning()) {

        manager.Update();
        manager.Draw();
    }

    CloseWindow();
    return 0;
}