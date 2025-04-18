#include "interface.hpp"
#include "states.hpp"

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

    // TOURNAMENT_08
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_1, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE2, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE3, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_3, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE4, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_4, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_08, DellState(STATE::TOURNAMENT_08, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_04
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_1, false, true),
        std::make_shared<TextBox>(BoxID::VALIDATE2, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_2, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_04, DellState(STATE::TOURNAMENT_04, screenObjs));
    screenObjs.clear();

    // TOURNAMENT_02
    screenObjs = {
        std::make_shared<SimpleText>("TORNEIO", TEXTBOX_FONTSIZE, SCREEN_POS_CENTER_TOP, false, false),
        std::make_shared<TextBox>(BoxID::VALIDATE1, std::vector<std::string>{"Avaliar", "Concluído"}, SCREEN_POS_CENTER_LEFT_1, false, true),
        std::make_shared<TextBox>(BoxID::BACK, std::vector<std::string>{"Cancelar"}, SCREEN_POS_CENTER_BOTTOM_RIGHT, false, true)

    };
    states.emplace(STATE::TOURNAMENT_02, DellState(STATE::TOURNAMENT_02, screenObjs));
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