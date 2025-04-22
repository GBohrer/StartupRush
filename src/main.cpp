#include <raylib.h>

#include "manager.hpp"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "STARTUP RUSH");
    SetTargetFPS(60);

    Manager& manager = Manager::GetManager();
    manager.Init();

    manager.CreateStartupSamples(2);

    while (manager.isRunning()) {

        manager.Update();
        manager.Draw();
    }

    CloseWindow();
    return 0;
}