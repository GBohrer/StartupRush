#include <raylib.h>

#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "manager.hpp"
#include "interface.hpp"

bool Manager::isRunning() {
    return this->run;
}

void Manager::Init() {
    this->run = true;
    this->states = StatesInit();
    this->currentState = states.at(STATE::ENTRY);
    this->lastState = states.at(STATE::ENTRY);
    this->rushGame.Init();
}

void Manager::Update() {

    if(IsKeyPressed(KEY_ESCAPE)) this->run = false;
    if(IsKeyPressed(KEY_P)) rushGame.PrintStartups();
    if(IsKeyPressed(KEY_CAPS_LOCK)) std::cout << this->lastState.state << std::endl;

    if (stateHandlers.find(this->currentState.state) != stateHandlers.end()) {
        stateHandlers[this->currentState.state](*this);
    } else {
        std::exit(-1);
    }

    // Atualiza as PopUpMessages atuais
    for (auto it = messages.begin(); it != messages.end();) {
        it->Update();
        if (it->IsExpired()) {
            it = messages.erase(it);
        } else {
            ++it;
        }
    }
}

void Manager::Draw() {
    BeginDrawing();
    ClearBackground(COLOR_BACKGROUND);

    // Renderiza UIObjects
    for (const auto& obj : this->currentState.screenObjs)
        obj->Draw();

    // Renderiza PopUpMessages
    for (auto PopM = messages.begin(); PopM != messages.end(); ++PopM) {
        PopM->Draw();
    }

    EndDrawing();
}

void Manager::Close() {
    this->run = false;
}

std::vector<std::shared_ptr<UIObject>>& Manager::GetUIObjects() {
    return this->currentState.screenObjs;
}

Tournament& Manager::GetRushGame() {
    return this->rushGame;
}

void Manager::ResetRushGame() {
    this->rushGame.ClearBattles();
    this->rushGame.ResetStartups();
}

void Manager::SetCurrentState(STATE state) {
    DellState s = states.at(state);
    this->currentState = s;
}

void Manager::UpdateLastState() {
    this->lastState = currentState;
}

void Manager::ReturnToLastState() {
    this->currentState = lastState;
}

void Manager::CreateMessage(PopUpMessage m) {
    if( (int)messages.size() < POP_UP_MESSAGES_MAX)
        messages.emplace_back(m);
}

bool Manager::HasMessages() {
    return (int)this->messages.size() > 0;
}

bool Manager::isPromptsOk() {
    bool needMessage = false;

    for (const auto& obj : GetUIObjects()) {
        PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());
        if (pb && pb->GetCurrentText().empty()) needMessage = true;
    }
    if(needMessage) {
        CreateMessage(PopUpMessage("Preencha todos os campos!", SCREEN_POS_CENTER_BOTTOM));
        return false;
    } else {
        return true;
    }
}


void Manager::ClearPromtps() {
    for (const auto& obj : GetUIObjects()) {
        PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());
        if (pb) pb->ResetText();
    }
}

bool Manager::isTournamentReady() {
    int total = rushGame.GetTotalStartups();

    if (total < 4) {
        CreateMessage(PopUpMessage("Adicione no mínimo 4 Startups!", SCREEN_POS_CENTER_BOTTOM));
        return false;
    } else if (!(total % 2 == 0)) {
        CreateMessage(PopUpMessage("Adicione mais 1 Startup!", SCREEN_POS_CENTER_BOTTOM));
        return false;
    } else if (total > 8) {
        CreateMessage(PopUpMessage("Máximo de Startups atingido!", SCREEN_POS_CENTER_BOTTOM));
        return false; 
    } else { return true; }
}

void Manager::CreateStartup() {

    if (rushGame.GetTotalStartups() == 8) return;

    PromptBox* pb;
    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(2).get());
    std::string name = pb->GetCurrentText();

    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(4).get());
    std::string slogan = pb->GetCurrentText();

    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(6).get());
    uint16_t year = static_cast<uint16_t>(std::atoi(pb->GetCurrentText().c_str()));

    rushGame.AddStartup(Startup(name, slogan, year));
}

void Manager::UpdateCurrentBattle(int battle_pos) {
    rushGame.SetCurrentBattle(rushGame.GetBattles()[battle_pos-1]);
}

// INTERFACE

void Handle_UI(Manager& manager, std::function<void(Box*)> callback) {
    for (const auto& obj : manager.GetUIObjects()) {
        if (obj && obj->isClickable) {
            TextBox* tb = dynamic_cast<TextBox*>(obj.get());
            PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());

            // Verifica TextBox
            if (tb) {
                if (CheckCollisionPointRec(GetMousePosition(), tb->GetBox())) {
                    tb->SetIsCursorOn(true);

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        //tb->SetNextText();
                        callback(tb);
                        return;
                    }
                } else if (tb) {
                    tb->SetIsCursorOn(false);
                }
            }

            // Verifica PromptBox
            if (pb) {
                if (CheckCollisionPointRec(GetMousePosition(), pb->GetBox())) {
                    pb->SetIsCursorOn(true);

                } else {
                    pb->SetIsCursorOn(false);
                }

                if(pb->isCursorOn) {
                    int key = GetKeyPressed();
                    while (key > 0) {
                        pb->UpdateText(key);
                        key = GetKeyPressed();
                    }
                }
            }
        }
    }
}


// State Handlers
std::map<STATE, std::function<void(Manager&)>> stateHandlers = {
    {STATE::ENTRY, Handle_ENTRY},
    {STATE::CREATE_STARTUP, Handle_CREATE_STARTUP},
    {STATE::TOURNAMENT_08, Handle_TOURNAMENT_08},
    {STATE::TOURNAMENT_06, Handle_TOURNAMENT_06},
    {STATE::TOURNAMENT_04, Handle_TOURNAMENT_04},
    {STATE::TOURNAMENT_02, Handle_TOURNAMENT_02},
    {STATE::BATTLE, Handle_BATTLE},
    {STATE::LEAVING, Handle_LEAVING},
};

void Handle_ENTRY(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::NEW_STARTUP:
                if(!(manager.GetRushGame().GetTotalStartups() == 8)) {
                    manager.SetCurrentState(STATE::CREATE_STARTUP);
                    return;
                } else {
                    manager.CreateMessage(PopUpMessage("Limite de Startups atingido!", SCREEN_POS_CENTER_BOTTOM));
                }
                break;
            case BoxID::BEGIN_TOURNAMENT:
                if(manager.isTournamentReady()){
                    manager.UpdateLastState();
                    int totalBattles = manager.GetRushGame().MakeBattles();
                    if (totalBattles == 4) manager.SetCurrentState(STATE::TOURNAMENT_08);
                    if (totalBattles == 3) manager.SetCurrentState(STATE::TOURNAMENT_06);
                    if (totalBattles == 2) manager.SetCurrentState(STATE::TOURNAMENT_04);
                    if (totalBattles == 1) manager.SetCurrentState(STATE::TOURNAMENT_02);
                    return;
                }
                break;
            case BoxID::EXIT:
                manager.SetCurrentState(STATE::LEAVING);
                return;
            default:
                break;
        }
    });
    PrintStartupsCount(manager.GetRushGame().GetTotalStartups());
}

void Handle_CREATE_STARTUP(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {

            case BoxID::CREATE:
                if (manager.isPromptsOk()) {
                    manager.CreateStartup();
                    manager.ClearPromtps();
                    manager.SetCurrentState(STATE::ENTRY);
                }
                return;

            case BoxID::BACK:
                manager.ClearPromtps();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
    });
}

void Handle_TOURNAMENT_08(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                manager.UpdateLastState();
                manager.UpdateCurrentBattle(1);
                manager.SetCurrentState(STATE::BATTLE);
                return;
            case BoxID::VALIDATE2:
                return;
            case BoxID::VALIDATE3:
                return;
            case BoxID::VALIDATE4:
                return;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_06(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                return;
            case BoxID::VALIDATE2:
                return;
            case BoxID::VALIDATE3:
                return;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_04(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                return;
            case BoxID::VALIDATE2:
                return;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_02(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                break;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_BATTLE(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::CREATE:
                // Salvar os dados
                // Contabilizar os pontos e definir qual startup competing = false
                manager.ReturnToLastState();
                return;
            case BoxID::BACK:
                // Reset das atribuições dos eventos
                manager.ReturnToLastState();
                return;
            default:
                break;
        }
    });
}

void Handle_LEAVING(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::NO:
                manager.SetCurrentState(STATE::ENTRY);
                return;
            case BoxID::YES:
                manager.Close();
                return;
            default:
                break;
        }
    });
}

// Debug Methods

void Manager::CreateStartupSamples(int total) {
    std::vector<Startup> samples;
    samples.emplace_back(Startup("Microsoft", "texto1", 1987));
    samples.emplace_back(Startup("Google", "texto2", 1997));
    samples.emplace_back(Startup("Nintendo", "texto3", 1963));
    samples.emplace_back(Startup("Rockstar", "texto4", 1999));
    samples.emplace_back(Startup("Amazon", "texto5", 2004));
    samples.emplace_back(Startup("Netflix", "texto6", 2005));
    samples.emplace_back(Startup("Meta", "texto7", 2008));
    samples.emplace_back(Startup("Apple", "texto1", 1979));

    for (int i=0 ; i < total; i++) {
        rushGame.AddStartup(samples[i]);
    }
}