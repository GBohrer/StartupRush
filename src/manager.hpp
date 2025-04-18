#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>
#include <map>
#include <functional>
#include <unordered_map>
#include <memory>

#include "interface.hpp"
#include "classes.hpp"

#define POP_UP_MESSAGES_MAX     2

enum STATE {
    ENTRY,
    CREATE_STARTUP,
    TOURNAMENT_01,
    TOURNAMENT_02,
    TOURNAMENT_03,
    LEAVING
};

typedef struct DellState {
    STATE state;
    std::vector<std::shared_ptr<UIObject>> screenObjs;

    DellState() = default;
    DellState(STATE s, std::vector<std::shared_ptr<UIObject>> so)
        : state(s), screenObjs(std::move(so)) {}

} DellState;

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
void Handle_TOURNAMENT_01(Manager& manager);
void Handle_TOURNAMENT_02(Manager& manager);
void Handle_TOURNAMENT_03(Manager& manager);
void Handle_LEAVING(Manager& manager);

extern std::map<STATE, std::function<void(Manager&)>> stateHandlers;
std::unordered_map<STATE, DellState> StatesInit ();

#endif