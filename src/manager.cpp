#include <raylib.h>

#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

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
    for (const auto& obj : this->currentState.screenObjs){
        obj->Draw();
    }

    // Renderiza PopUpMessages
    for (auto PopM = messages.begin(); PopM != messages.end(); ++PopM) {
        PopM->Draw(MAROON);
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

    //vector auxiliar com os nomes de startups ja adicionados
    std::vector<std::string> startupNames;
    for(const auto& [startup, value, events] : rushGame.GetStartups()){
        startupNames.emplace_back(startup.getName());
    }

    for (const auto& obj : GetUIObjects()) {
        PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());

        if (pb){
            if(pb->GetCurrentText().empty()) {
                CreateMessage(PopUpMessage("Preencha todos os campos!", SCREEN_POS_CENTER_BOTTOM));
                return false;

            } else if (std::find(startupNames.begin(), startupNames.end(), pb->GetCurrentText()) != startupNames.end()) {
                CreateMessage(PopUpMessage("Nome já registrado!", SCREEN_POS_CENTER_BOTTOM));
                return false;
            }
        }
    }
    return true;
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

void Manager::ResetBattle(bool shouldResetPoints) {

    Battle& currentBattle = rushGame.GetCurrentBattle();

    for (const auto& obj : GetUIObjects()) {
        BattleTextBox* btb = dynamic_cast<BattleTextBox*>(obj.get());

        if(btb && btb->isPressed()) {
            if(shouldResetPoints){
                auto& startup = (btb->GetID() == BoxID::EVENT_A)
                ? currentBattle.GetStartupA()
                : currentBattle.GetStartupB();

                auto& eventValue = btb->GetValue();
                auto currentPoints = rushGame.GetStartupPointsByName(startup.getName());
                
                int16_t updatedPoints = currentPoints - eventValue;
                rushGame.UpdateStartupPoints(startup, updatedPoints);
            }
            btb->SetPressed(false);
        }
    }

}

void Manager::UpdateCurrentBattle(int battle_pos) {
    rushGame.SetCurrentBattle(rushGame.GetBattles()[battle_pos-1]);
}

void Manager::UpdateCurrentBattlePoints(BattleTextBox* btb) {
    Battle& currentBattle = rushGame.GetCurrentBattle();

    auto& startup = (btb->GetID() == BoxID::EVENT_A)
    ? currentBattle.GetStartupA()
    : currentBattle.GetStartupB();

    auto currentPoints = rushGame.GetStartupPointsByName(startup.getName());
    BattleEvent be = BattleEvent(btb->GetEventID(), btb->GetValue());

    // Acrescenta/decrementa pontos de acordo com o botão do BattleEvent (pressed)
    auto& eventValue = btb->GetValue();
    int16_t updatedPoints = currentPoints + (btb->isPressed() ? eventValue : -eventValue);
    rushGame.UpdateStartupPoints(startup, updatedPoints);
    rushGame.UpdateStartupBattleEvent(startup, be);

    // Adiciona o BattleEvent ao vector da tuple da startup (para relatório)
}

void Manager::SelectWinner() {
    //Battle& currentBattle = rushGame.GetCurrentBattle();
}


// INTERFACE

void Handle_UI(Manager& manager, std::function<void(Box*)> callback) {
    for (const auto& obj : manager.GetUIObjects()) {
        if (obj && obj->isClickable) {
            TextBox* tb = dynamic_cast<TextBox*>(obj.get());
            PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());
            BattleTextBox* btb = dynamic_cast<BattleTextBox*>(obj.get());

            // Verifca BattleTextBox
            if (btb) {
                if (CheckCollisionPointRec(GetMousePosition(), btb->GetBox())) {
                    btb->SetIsCursorOn(true);
                    PrintEventDescription(btb->GetEventID());

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        btb->TogglePressed();
                        callback(btb);
                        return;
                    }
                } else if (btb) {
                    btb->SetIsCursorOn(false);
                }
            }

            // Verifica TextBox
            if (tb) {
                if (CheckCollisionPointRec(GetMousePosition(), tb->GetBox())) {
                    tb->SetIsCursorOn(true);

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        
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