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

std::vector<std::shared_ptr<UIObject>>& Manager::GetUIObjectsFromState(const DellState& ds) {
    return states[ds.state].screenObjs;
}

void Manager::BlockTextBoxFromState(DellState s, int index) {
    for (auto& obj : GetUIObjectsFromState(s)) {
        TextBox* tb = dynamic_cast<TextBox*>(obj.get());

        if(tb && tb->pressed && tb->GetID() == index+1) {
            tb->isClickable = false;
            tb->SetCurrentText(1);
        }
    }
}


Tournament& Manager::GetRushGame() {
    return this->rushGame;
}

void Manager::ResetRushGame() {
    this->rushGame.ClearBattles();
    this->rushGame.ResetStartups();
    this->states = StatesInit();
}

void Manager::SetRushGameChampion() {
    int16_t max = 0;
    StartupEntry champion;
    for(const auto& startup : rushGame.GetStartups()) {
        if (startup.totalPoints > max) {
            max = startup.totalPoints;
            champion = startup;
        }
    }
    rushGame.SetChampion(champion);
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
    for(const auto& [startup, value, status, events] : rushGame.GetStartups()){
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

void Manager::ResetBattle(bool shouldResetAll) {

    Battle& currentBattle = rushGame.GetCurrentBattle();

    for (const auto& obj : GetUIObjects()) {
        BattleTextBox* btb = dynamic_cast<BattleTextBox*>(obj.get());

        // Reset dos botões de eventos + retirada dos pontos baseada nestes botões
        if(btb && btb->isPressed()) {

            // Retira os pontos
            if(shouldResetAll){
                auto& startup_entry = (btb->GetID() == BoxID::EVENT_A)
                ? currentBattle.GetStartupA()
                : currentBattle.GetStartupB();

                auto& eventValue = btb->GetValue();

                rushGame.AddStartupPoints(startup_entry.startup, -eventValue);
                rushGame.ClearStartupBattleEvents(startup_entry.startup);
            }
            // Reset do botão
            btb->SetPressed(false);
        }
    }

    // Atualiza a tela do torneio apenas quando os pontos são salvos!
    //if (shouldResetAll) ResetTextBoxFromState(lastState);
}

void Manager::UpdateCurrentBattle(int battle_pos) {
    rushGame.SetCurrentBattle(rushGame.GetBattles()[battle_pos-1]);
    rushGame.SetCurrentBattleIndex(battle_pos-1);
}

void Manager::UpdateCurrentBattlePoints(BattleTextBox* btb) {
    Battle& currentBattle = rushGame.GetCurrentBattle();

    auto& startup_entry = (btb->GetID() == BoxID::EVENT_A)
    ? currentBattle.GetStartupA()
    : currentBattle.GetStartupB();

    BattleEvent be = BattleEvent(btb->GetEventID(), btb->GetValue());

    // Acrescenta/decrementa pontos de acordo com o botão do BattleEvent (pressed)
    auto& eventValue = btb->GetValue();
    int16_t updatedValue = (btb->isPressed() ? eventValue : -eventValue);
    rushGame.AddStartupPoints(startup_entry.startup, updatedValue);
    rushGame.UpdateStartupBattleEvent(startup_entry.startup, be);
}

void Manager::SelectWinner() {
    auto& currentBattle = rushGame.GetCurrentBattle();
    auto& startupA_entry = currentBattle.GetStartupA();
    auto& startupB_entry = currentBattle.GetStartupB();
    auto& pointsA = rushGame.GetStartupPointsByName(startupA_entry.startup.getName());
    auto& pointsB = rushGame.GetStartupPointsByName(startupB_entry.startup.getName());

    StartupEntry winner;
    StartupEntry loser;

    if (pointsA == pointsB) {
        CreateMessage(PopUpMessage("Shark Fight...", SCREEN_POS_CENTER_6));

        if (rand() % 2 == 0) {
            winner = startupA_entry;
            loser  = startupB_entry;
        } else {
            winner = startupB_entry;
            loser  = startupA_entry;
        }
        rushGame.AddStartupPoints(winner.startup, 2);

    } else if (pointsA > pointsB) {
        winner = startupA_entry;
        loser = startupB_entry;
    } else {
        winner = startupB_entry;
        loser = startupA_entry;
    }

    rushGame.AddStartupPoints(winner.startup, 30);
    rushGame.UpdateStartupStatus(winner.startup, Status::AVALIABLE);
    rushGame.UpdateStartupStatus(loser.startup, Status::DESQUALIFIED);

    currentBattle.SetStatus(BattleStatus::Complete);
    rushGame.UpdateBattles(currentBattle);

    BlockTextBoxFromState(lastState, rushGame.GetCurrentBattleIndex());
}

bool Manager::isAllBattlesCompleted() {
    for (const auto& battle : rushGame.GetBattles()) {
        if (battle.GetStatus() == BattleStatus::Pending) return false;
    }
    //this->states = StatesInit();

    return true;
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
                        tb->SetPressed(true);
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
    samples.emplace_back(Startup("Microsoft", "Be what is next.", 1975));
    samples.emplace_back(Startup("Google", "Do not be evil.", 1998));
    samples.emplace_back(Startup("Nintendo", "There is no play like it.", 1889));
    samples.emplace_back(Startup("Rockstar", "Making fun of the world since 1998.", 1998));
    samples.emplace_back(Startup("Amazon", "Work hard. Have fun. Make history.", 1994));
    samples.emplace_back(Startup("Netflix", "See what is next.", 1997));
    samples.emplace_back(Startup("Meta", "Move fast.", 2004));
    samples.emplace_back(Startup("Apple", "Think different.", 1976));
    samples.emplace_back(Startup("IBM", "Think.", 1911));
    samples.emplace_back(Startup("Intel", "Intel Inside.", 1968));
    samples.emplace_back(Startup("Adobe", "Changing the world through digital experiences.", 1982));
    samples.emplace_back(Startup("Oracle", "Data is the new oil.", 1977));
    samples.emplace_back(Startup("TikTok", "Make every second count.", 2016));
    samples.emplace_back(Startup("Naughty Dog", "You cannot stop this.", 1984));
    samples.emplace_back(Startup("Toyota", "Let is Go Places.", 1937));
    samples.emplace_back(Startup("Airbnb", "Belong anywhere.", 2008));
    samples.emplace_back(Startup("Discord", "Your place to talk.", 2015));
    samples.emplace_back(Startup("Sony", "Make. Believe.", 1946));
    
    total = std::min(total, static_cast<int>(samples.size()));

    for (int i=0 ; i < total; i++) {
        int randomIndex = rand() % samples.size();
        rushGame.AddStartup(samples[randomIndex]);
        samples.erase(samples.begin()+randomIndex);
    }
}