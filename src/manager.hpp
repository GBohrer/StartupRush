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
        std::vector<std::shared_ptr<UIObject>>& GetUIObjectsFromState(const DellState& state);
        void BlockTextBoxFromState(DellState s, int index);

        void SetCurrentState(STATE state);
        void UpdateLastState();
        void ReturnToLastState();

        void CreateMessage(PopUpMessage m);
        bool HasMessages();

        Tournament& GetRushGame();
        bool isTournamentReady();
        void ResetRushGame();
        void SetRushGameChampion();

        bool isPromptsOk();
        void ClearPromtps();

        void CreateStartup();
        void CreateStartupSamples(int total);

        void ResetBattle(bool shouldResetAll);
        void UpdateCurrentBattle(int battle_pos);
        void UpdateCurrentBattlePoints(BattleTextBox* btb);
        void SelectWinner();
        bool isAllBattlesCompleted();
        
    private:
        std::unordered_map<STATE, DellState> states;
        std::vector<PopUpMessage> messages;
        DellState currentState;
        DellState lastState;
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
void Handle_BATTLE(Manager& manager);
void Handle_CHAMPION(Manager& manager);
void Handle_RESULTS(Manager& manager);
void Handle_LEAVING(Manager& manager);

extern std::map<STATE, std::function<void(Manager&)>> stateHandlers;


#endif