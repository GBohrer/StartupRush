#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "interface.hpp"
#include "states.hpp"
#include "classes.hpp"

#define POP_UP_MESSAGES_MAX     2


class Manager {
    public:
        static Manager& GetManager() {
            static Manager instance;
            return instance;
        }

        bool isRunning();
        void Init();
        void Update();
        void Draw();
        void Close();

        std::vector<std::shared_ptr<UIObject>>& GetUIObjects();
        Tournament& GetRushGame();

        void SetCurrentState(STATE state);

        void CreateMessage(PopUpMessage m);
        bool HasMessages();

        bool isPromptsOk();
        void ClearPromtps();

        bool isTournamentReady();
        void CreateStartup();

        
    private:
        std::unordered_map<STATE, DellState> states;
        std::vector<PopUpMessage> messages;
        DellState currentState;
        bool run;

        Tournament rushGame;
};

// Interface

void Handle_UI(Manager& manager, std::function<void(Box*)> callback);


// State Handlers

void Handle_ENTRY(Manager& manager);
void Handle_CREATE_STARTUP(Manager& manager);
void Handle_TOURNAMENT_08(Manager& manager);
void Handle_TOURNAMENT_06(Manager& manager);
void Handle_TOURNAMENT_04(Manager& manager);
void Handle_TOURNAMENT_02(Manager& manager);
void Handle_LEAVING(Manager& manager);

extern std::map<STATE, std::function<void(Manager&)>> stateHandlers;

#endif