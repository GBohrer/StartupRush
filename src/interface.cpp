#include <raylib.h>
#include <string>
#include <vector>
#include <sstream>

#include "interface.hpp"


// SIMPLETEXT

SimpleText::SimpleText(std::string text, int fontSize, Vector2 pos, bool b, bool c) {
    this->isBlinkable = b;
    this->isClickable = c;
    this->isCursorOn = false;
    this->text = text;
    this->fontSize = fontSize;
    this->textWidth = MeasureText(text.c_str(), fontSize);
    this->pos = {pos.x - textWidth/2, pos.y};
}

void SimpleText::Draw() {
    Color color;
    if (isCursorOn) { color = COLOR_MOUSE_HOVER; } else { color = LIGHTGRAY; }
    DrawText(text.c_str(), pos.x, pos.y, fontSize, color);
}

// BOX

Rectangle Box::GetBox() {
    Rectangle r = {pos.x, pos.y, (float)width, (float)height};
    return r;
}

BoxID Box::GetID() {
    return this->id;
}

void Box::SetIsCursorOn(bool b) {
    this->isCursorOn = b;
}

// TEXTBOX

TextBox::TextBox(BoxID id, std::vector<std::string> strings, Vector2 pos, bool b, bool c) {
    this->id = id;
    this->width = TEXTBOX_WIDTH;
    this->height = TEXTBOX_HEIGHT;
    this->isCursorOn = false;
    this-> isBlinkable = b;
    this->isClickable = c;
    this->pos = {pos.x - width/2, pos.y};
    this->currentTextIndex = 0;
    
    texts.reserve(strings.size());

    for(const std::string& s : strings) {
        int new_fontsize = TEXTBOX_FONTSIZE;
        int text_width = MeasureText(s.c_str(), new_fontsize);

        while( text_width >= TEXTBOX_WIDTH) {
            new_fontsize -= 4;
            text_width = MeasureText(s.c_str(), new_fontsize);
        }

        Vector2 t_pos;
        t_pos = {pos.x, pos.y + (height - new_fontsize)/2};
        texts.emplace_back(SimpleText(s, new_fontsize, t_pos, b, c));
    }
}

void TextBox::SetNextText() {
    this->currentTextIndex = (currentTextIndex + 1) % texts.size();
}

std::string& TextBox::GetText() {
    return texts[currentTextIndex].text;
}

void TextBox::SetIsCursorOn(bool b) {
    this->isCursorOn = b;
    texts[currentTextIndex].isCursorOn = b;
}

void TextBox::Draw() {
    Color color;
    if (isCursorOn) { color = COLOR_MOUSE_HOVER; } else { color = LIGHTGRAY; }
    DrawRectangleLinesEx({pos.x, pos.y, (float)width, (float)height}, BOX_THICKNESS, color);
    texts[currentTextIndex].Draw();
}

//BATTLE TEXTBOX

BattleTextBox::BattleTextBox(BoxID boxID, std::vector<std::string> strings, Vector2 pos, bool b, bool c, EventID eID, int8_t v)
    : TextBox(boxID, strings, pos, b, c), eventID(eID), value(v) {}

EventID BattleTextBox::GetEventID() {
    return eventID;
}

int8_t BattleTextBox::GetValue() {
    return value;
}


// PROMPTBOX

PromptBox::PromptBox(Vector2 pos, bool b, bool c) {
    this->id = BoxID::PROMPT;
    this->width = TEXTBOX_WIDTH * 2.3;
    this->height = TEXTBOX_HEIGHT;
    this->isCursorOn = false;
    this->isBlinkable = b;
    this->isClickable = c;
    this->pos = {pos.x - width / 2, pos.y};
    this->currentText = "";
    this->maxCharacters = 20;
}

void PromptBox::SetIsCursorOn(bool b) {
    this->isCursorOn = b;
}

void PromptBox::UpdateText(int key) {
    if (key == KEY_BACKSPACE && !currentText.empty()) {
        currentText.pop_back();

    } else if ((int)currentText.size() < maxCharacters && key >= 32 && key <= 126) {
        currentText.push_back(static_cast<char>(key));
    }
}

std::string PromptBox::GetCurrentText() {
    return this->currentText;
}

void PromptBox::ResetText() {
    this->currentText.clear();
}

void PromptBox::Draw() {
    Color color;
    if (isCursorOn) { color = COLOR_MOUSE_HOVER; } else { color = LIGHTGRAY; }
    DrawRectangleLinesEx({pos.x, pos.y, (float)width, (float)height}, BOX_THICKNESS, color);

    Vector2 textPos = {pos.x + PROMPTBOX_PADDING, pos.y + (height - TEXTBOX_FONTSIZE) / 2};
    DrawText(currentText.c_str(), textPos.x, textPos.y, TEXTBOX_FONTSIZE, color);
}


// INFO RENDERING

void PrintStartupsCount(int total) {
    std::string message = "Startups: ";
    message.append(std::to_string(total));
    DrawText(message.c_str(), SCREEN_POS_CENTER_BOTTOM_RIGHT.x-100, SCREEN_POS_CENTER_BOTTOM_RIGHT.y, TEXTBOX_FONTSIZE, LIGHTGRAY);
}

void PrintAllStartupsInfo(std::vector<std::tuple<Startup, uint16_t>> startups) {
    
    int offset = 0;
    for(const auto& [startup, value] : startups) {
        const char *name = (startup.getName()).c_str();
        const char *points = (std::to_string(value)).c_str();
        DrawText(name, SCREEN_POS_CENTER_TOP.x, SCREEN_POS_CENTER_1.y + offset, TEXTBOX_FONTSIZE, LIGHTGRAY);
        DrawText(points, SCREEN_POS_CENTER_TOP.x + MeasureText(name, TEXTBOX_FONTSIZE) + 20, SCREEN_POS_CENTER_1.y + offset, TEXTBOX_FONTSIZE, LIGHTGRAY);

        offset += TEXTBOX_FONTSIZE + 5;
    }
}

void PrintBattles(Tournament t) {
    std::stringstream stream;
    std::string text;
    int offset = 0;
    const auto& battles = t.GetBattles();

    for (size_t i=0; i < battles.size(); i++) {
        const auto& startupA = std::get<0>(battles[i].GetStartupA());
        std::string nameA = startupA.getName();
        const auto& startupB = std::get<0>(battles[i].GetStartupB());
        std::string nameB = startupB.getName();

        stream << "Batalha " << i+1 << ": " << nameA << "  Vs.  " << nameB;
        text = stream.str();

        DrawText(text.c_str(), SCREEN_POS_CENTER_2.x - 300, SCREEN_POS_CENTER_2.y + offset, TEXTBOX_FONTSIZE+10, LIGHTGRAY);
        
        text.clear(); stream.str("");
        offset += 70;
    }
}

void PrintCurrentBattleAndPoints(Tournament t) {

    Battle& battle = t.GetCurrentBattle();

    const auto& [startupA, battleEventsA] = battle.GetStartupA();
    const char* nameA = startupA.getName().c_str();
    
    const char* pointsA = std::to_string(t.GetStartupPointsByName(nameA)).c_str();
    
    DrawText(nameA,SCREEN_POS_CENTER_LEFT_1.x - MeasureText(nameA, TEXTBOX_FONTSIZE_2)/2, SCREEN_POS_CENTER_LEFT_1.y, TEXTBOX_FONTSIZE_2, LIGHTGRAY);
    DrawText(pointsA,SCREEN_POS_CENTER_LEFT_2.x - MeasureText(pointsA, TEXTBOX_FONTSIZE_2)/2, SCREEN_POS_CENTER_LEFT_2.y, TEXTBOX_FONTSIZE_2, LIGHTGRAY);
    
    const auto& [startupB, battleEventsB] = battle.GetStartupB();
    const char* nameB = startupB.getName().c_str();
    const char* pointsB = std::to_string(t.GetStartupPointsByName(nameB)).c_str();

    DrawText(nameB,SCREEN_POS_CENTER_RIGHT_1.x - MeasureText(nameB, TEXTBOX_FONTSIZE_2)/2, SCREEN_POS_CENTER_RIGHT_1.y, TEXTBOX_FONTSIZE_2, LIGHTGRAY);
    DrawText(pointsB,SCREEN_POS_CENTER_RIGHT_2.x - MeasureText(pointsB, TEXTBOX_FONTSIZE_2)/2, SCREEN_POS_CENTER_RIGHT_2.y, TEXTBOX_FONTSIZE_2, LIGHTGRAY);
    
    // Renderizar os pontos de CADA BattleEvent ao lado (esquerdo ou direito) de suas respectivas TextBox
}
