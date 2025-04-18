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
    this->rushGame = Tournament();
}

void Manager::Update() {

    if(IsKeyPressed(KEY_ESCAPE)) this->run = false;
    if(IsKeyPressed(KEY_P)) rushGame.PrintStartups();

    if (stateHandlers.find(this->currentState.state) != stateHandlers.end()) {
        stateHandlers[this->currentState.state](*this);
    } else {
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

void Manager::SetCurrentState(STATE state) {
    DellState s = states.at(state);
    this->currentState = s;
}

void Manager::CreateMessage(PopUpMessage m) {
    if( (int)messages.size() < POP_UP_MESSAGES_MAX)
        messages.emplace_back(m);
}

bool Manager::HasMessages() {
    return (int)this->messages.size() > 0;
}

bool Manager::isPromptsOk() {
    for (const auto& obj : GetUIObjects()) {
        PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());
        if (pb) {
            if (pb->GetCurrentText().empty()) {
                CreateMessage(PopUpMessage("Preencha todos os campos!", SCREEN_POS_CENTER_BOTTOM));
                return false;
            }
            return true;
        } else { continue; }
    }
    return true;
}

void Manager::ClearPromtps() {
    for (const auto& obj : GetUIObjects()) {
        PromptBox* pb = dynamic_cast<PromptBox*>(obj.get());
        if (pb) pb->ResetText();
    }
}

void Manager::CreateStartup() {

    PromptBox* pb;
    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(2).get());
    std::string name = pb->GetCurrentText();

    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(4).get());
    std::string slogan = pb->GetCurrentText();

    pb = dynamic_cast<PromptBox*>(GetUIObjects().at(6).get());
    uint16_t year = static_cast<uint16_t>(std::atoi(pb->GetCurrentText().c_str()));

    rushGame.AddStartup(Startup(name, slogan, year));
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
                        tb->SetNextText();
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
    {STATE::TOURNAMENT_01, Handle_TOURNAMENT_01},
    {STATE::TOURNAMENT_02, Handle_TOURNAMENT_02},
    {STATE::TOURNAMENT_03, Handle_TOURNAMENT_03},
    {STATE::LEAVING, Handle_LEAVING},
};

void Handle_ENTRY(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::NEW_STARTUP:
                // Checar se max_startups >= 8
                if(!manager.HasMessages()){
                    manager.SetCurrentState(STATE::CREATE_STARTUP);
                    return;
                }
                break;
            case BoxID::BEGIN_TOURNAMENT:
                // Checar se startups.size() >= 4 && startups.size() % 2 == 0
                if(!manager.HasMessages()){
                    //manager.SetCurrentState(STATE::TOURNAMENT_01);
                    return;
                }
            case BoxID::EXIT:
                manager.SetCurrentState(STATE::LEAVING);
                return;
            default:
                break;
        }
    });
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

void Handle_TOURNAMENT_01(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {

            default:
                break;
        }
    });
}

void Handle_TOURNAMENT_02(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {

            default:
                break;
        }
    });
}

void Handle_TOURNAMENT_03(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {

            default:
                break;
        }
    });
}

void Handle_LEAVING(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::NO:
                manager.Init();
                return;
            case BoxID::YES:
                manager.Close();
                return;
            default:
                break;
        }
    });
}


std::unordered_map<STATE, DellState> StatesInit () {
    std::unordered_map<STATE, DellState> states;
    std::vector<std::shared_ptr<UIObject>> screenObjs;

    // ENTRY
    screenObjs = {
        std::make_shared<SimpleText>("STARTUP RUSH", TITLE_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::BEGIN_TOURNAMENT, std::vector<std::string>{"Começar Torneio"}, SCREEN_POS_CENTER_1, false, true),
        std::make_shared<TextBox>(BoxID::NEW_STARTUP, std::vector<std::string>{"Adicionar Startup"}, SCREEN_POS_CENTER_2, false, true),
        std::make_shared<TextBox>(BoxID::EXIT, std::vector<std::string>{"Sair"}, SCREEN_POS_CENTER_3, false, true)

    };
    states.emplace(STATE::ENTRY, DellState(STATE::ENTRY, screenObjs));
    screenObjs.clear();

    // CREATE_STARTUP
    screenObjs = {
        std::make_shared<SimpleText>("INSIRA OS DADOS DA STARTUP", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<SimpleText>("Nome:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_1, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_1, true, true),
        std::make_shared<SimpleText>("Slogan:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_2, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_2, true, true),
        std::make_shared<SimpleText>("Ano:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_3, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_3, true, true),
        std::make_shared<TextBox>(BoxID::CREATE, std::vector<std::string>{"Adicionar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Voltar"}, SCREEN_POS_CENTER_BOTTOM_LEFT, false, true)

    };
    states.emplace(STATE::CREATE_STARTUP, DellState(STATE::CREATE_STARTUP, screenObjs));
    screenObjs.clear();

    // LEAVING
    screenObjs = {
        std::make_shared<SimpleText>("SAIR DO SISTEMA?", TITLE_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::YES, std::vector<std::string>{"Sim"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true),
        std::make_shared<TextBox>(BoxID::NO, std::vector<std::string>{"Não"}, SCREEN_POS_CENTER_BOTTOM_LEFT, false, true)
    };
    states.emplace(STATE::LEAVING, DellState(STATE::LEAVING, screenObjs));
    screenObjs.clear(); 
    
    return states;
}