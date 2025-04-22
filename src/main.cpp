// STARTUP RUSH - V1.0

#include <raylib.h>

#include "manager.hpp"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "STARTUP RUSH");
    SetTargetFPS(60);

    Manager& manager = Manager::GetManager();
    manager.Init();

    while (manager.isRunning()) {

        manager.Update();
        manager.Draw();
    }

    CloseWindow();
    return 0;
}