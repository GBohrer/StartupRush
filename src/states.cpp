#include "interface.hpp"
#include "states.hpp"
#include "manager.hpp"

std::unordered_map<STATE, DellState> StatesInit () {
    std::unordered_map<STATE, DellState> states;
    std::vector<std::shared_ptr<UIObject>> screenObjs;

    // ENTRY
    screenObjs = {
        std::make_shared<SimpleText>("STARTUP RUSH", TITLE_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::BEGIN_TOURNAMENT, std::vector<std::string>{"Começar Torneio"}, SCREEN_POS_CENTER_3, false, true),
        std::make_shared<TextBox>(BoxID::NEW_STARTUP, std::vector<std::string>{"Adicionar Startup"}, SCREEN_POS_CENTER_4, false, true),
        std::make_shared<TextBox>(BoxID::EXIT, std::vector<std::string>{"Sair"}, SCREEN_POS_CENTER_5, false, true)

    };
    states.emplace(STATE::ENTRY, DellState(STATE::ENTRY, screenObjs));
    screenObjs.clear();

    // CREATE_STARTUP
    screenObjs = {
        std::make_shared<SimpleText>("INSIRA OS DADOS DA STARTUP", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<SimpleText>("Nome:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_3, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_3, true, true),
        std::make_shared<SimpleText>("Slogan:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_4, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_4, true, true),
        std::make_shared<SimpleText>("Ano:", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_LEFT_5, false, false),
        std::make_shared<PromptBox>(SCREEN_POS_CENTER_5, true, true),
        std::make_shared<TextBox>(BoxID::CREATE, std::vector<std::string>{"Adicionar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Voltar"}, SCREEN_POS_CENTER_BOTTOM_LEFT, false, true)

    };
    states.emplace(STATE::CREATE_STARTUP, DellState(STATE::CREATE_STARTUP, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_08
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE2, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_3, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE3, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_4, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE4, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_5, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_08, DellState(STATE::TOURNAMENT_08, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_06
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE2, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_3, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE3, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_4, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_06, DellState(STATE::TOURNAMENT_06, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_04
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE2, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_3, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_04, DellState(STATE::TOURNAMENT_04, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_02
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_02, DellState(STATE::TOURNAMENT_02, screenObjs));
    screenObjs.clear();

    // BATTLE
    screenObjs = {
        // STARTUP A
        std::make_shared<BattleTextBox>(BoxID::GOODPITCH_A, std::vector<std::string>{"Bom Pitch"}, SCREEN_POS_CENTER_LEFT_3, false, true, EventID::GoodPitch, 6),
        std::make_shared<BattleTextBox>(BoxID::BUGS_A, std::vector<std::string>{"Bugs"}, SCREEN_POS_CENTER_LEFT_4, false, true, EventID::WithBugs, -4),
        std::make_shared<BattleTextBox>(BoxID::USERS_A, std::vector<std::string>{"Boa Tração"}, SCREEN_POS_CENTER_LEFT_5, false, true, EventID::UserDriven, 3),
        std::make_shared<BattleTextBox>(BoxID::ANGRY_A, std::vector<std::string>{"Investidor :("}, SCREEN_POS_CENTER_LEFT_6, false, true, EventID::AngryInvestor, -6),
        std::make_shared<BattleTextBox>(BoxID::FAKENEWS_A, std::vector<std::string>{"Fake News"}, SCREEN_POS_CENTER_LEFT_7, false, true, EventID::FakeNews, -8),

        // STARTUP B
        std::make_shared<BattleTextBox>(BoxID::GOODPITCH_B, std::vector<std::string>{"Bom Pitch"}, SCREEN_POS_CENTER_RIGHT_3, false, true, EventID::GoodPitch, 6),
        std::make_shared<BattleTextBox>(BoxID::BUGS_B, std::vector<std::string>{"Bugs"}, SCREEN_POS_CENTER_RIGHT_4, false, true, EventID::WithBugs, -4),
        std::make_shared<BattleTextBox>(BoxID::USERS_B, std::vector<std::string>{"Boa Tração"}, SCREEN_POS_CENTER_RIGHT_5, false, true, EventID::UserDriven, 3),
        std::make_shared<BattleTextBox>(BoxID::ANGRY_B, std::vector<std::string>{"Investidor :("}, SCREEN_POS_CENTER_RIGHT_6, false, true, EventID::AngryInvestor, -6),
        std::make_shared<BattleTextBox>(BoxID::FAKENEWS_B, std::vector<std::string>{"Fake News"}, SCREEN_POS_CENTER_RIGHT_7, false, true, EventID::FakeNews, -8),

        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Voltar"}, SCREEN_POS_CENTER_BOTTOM_LEFT, false, true),
        std::make_shared<TextBox>(BoxID::CREATE, std::vector<std::string>{"Salvar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true) 
    };
    states.emplace(STATE::BATTLE, DellState(STATE::BATTLE, screenObjs));
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
                manager.UpdateCurrentBattle(1);
                break;
            case BoxID::VALIDATE2:
                manager.UpdateCurrentBattle(2);
                break;
            case BoxID::VALIDATE3:
                manager.UpdateCurrentBattle(3);
                break;
            case BoxID::VALIDATE4:
                manager.UpdateCurrentBattle(4);
                break;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
        manager.UpdateLastState();
        manager.SetCurrentState(STATE::BATTLE);
        return;
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_06(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                manager.UpdateCurrentBattle(1);
                break;
            case BoxID::VALIDATE2:
                manager.UpdateCurrentBattle(2);
                break;
            case BoxID::VALIDATE3:
                manager.UpdateCurrentBattle(3);
                break;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
        manager.UpdateLastState();
        manager.SetCurrentState(STATE::BATTLE);
        return;
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_04(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                manager.UpdateCurrentBattle(1);
                break;
            case BoxID::VALIDATE2:
                manager.UpdateCurrentBattle(2);
                break;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
        manager.UpdateLastState();
        manager.SetCurrentState(STATE::BATTLE);
        return;
    });
    PrintBattles(manager.GetRushGame());
}

void Handle_TOURNAMENT_02(Manager& manager) {
    Handle_UI(manager, [&manager](Box* tb) {
        switch(tb->GetID()) {
            case BoxID::VALIDATE1:
                manager.UpdateCurrentBattle(1);
                break;
            case BoxID::BACK:
                manager.ResetRushGame();
                manager.SetCurrentState(STATE::ENTRY);
                return;
            default:
                break;
        }
        manager.UpdateLastState();
        manager.SetCurrentState(STATE::BATTLE);
        return;
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
                BattleTextBox* btb = dynamic_cast<BattleTextBox*>(tb);
                manager.UpdateCurrentBattlePoints(btb->GetEventID(), btb->GetValue());
                break;
        }
    });
    PrintCurrentBattleAndPoints(manager.GetRushGame());
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
