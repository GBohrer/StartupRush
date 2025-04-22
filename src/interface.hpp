#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>
#include <raylib.h>

#include "classes.hpp"

#define SCREEN_WIDTH                1920
#define SCREEN_HEIGHT               1080

#define TEXTBOX_WIDTH               310
#define TEXTBOX_HEIGHT              70
#define TEXTBOX_FONTSIZE            35
#define TEXTBOX_FONTSIZE_2          65
#define TEXTBOX_FONTSIZE_3          20
#define BOX_THICKNESS               5
#define TITLE_FONTSIZE              120
#define PROMPTBOX_PADDING           15
#define BOX_OFFSET                  90

#define COLOR_BACKGROUND            (Color){ 10, 10, 40, 255}
#define COLOR_MOUSE_HOVER           (Color){ 50, 240, 48, 255}

const Vector2 SCREEN_POS_CENTER_1            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 400};
const Vector2 SCREEN_POS_CENTER_LEFT_1       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 400};
const Vector2 SCREEN_POS_CENTER_RIGHT_1      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 400};
const Vector2 SCREEN_POS_CENTER_2            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_LEFT_2       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_RIGHT_2      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_3            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 200};
const Vector2 SCREEN_POS_CENTER_LEFT_3       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 200};
const Vector2 SCREEN_POS_CENTER_RIGHT_3      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 200};
const Vector2 SCREEN_POS_CENTER_4            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 100};
const Vector2 SCREEN_POS_CENTER_LEFT_4       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 100};
const Vector2 SCREEN_POS_CENTER_RIGHT_4      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 100};
const Vector2 SCREEN_POS_CENTER_5            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_LEFT_5       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_RIGHT_5      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_6            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 100};
const Vector2 SCREEN_POS_CENTER_LEFT_6       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 100};
const Vector2 SCREEN_POS_CENTER_RIGHT_6      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 + 100};
const Vector2 SCREEN_POS_CENTER_7            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 200};
const Vector2 SCREEN_POS_CENTER_LEFT_7       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 200};
const Vector2 SCREEN_POS_CENTER_RIGHT_7      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 + 200};
const Vector2 SCREEN_POS_CENTER_TOP          = {(float)SCREEN_WIDTH / 2, 50};
const Vector2 SCREEN_POS_CENTER              = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_BOTTOM       = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_CENTER_BOTTOM_LEFT  = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_CENTER_BOTTOM_RIGHT = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_RIGHT_TOP           = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_RIGHT               = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_RIGHT_BOTTOM        = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_LEFT_TOP            = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_LEFT                = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_LEFT_BOTTOM         = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};


enum BoxID {
    BEGIN_TOURNAMENT,
    VALIDATE1,              // NÃO MUDAR INDEX DOS VALIDATE!!!
    VALIDATE2,
    VALIDATE3,
    VALIDATE4,
    NEW_STARTUP,
    CREATE,
    SELECTOR,
    EXIT,
    BACK,
    YES,
    NO,
    PROMPT,
    CONTENT,
    EVENT_A,
    EVENT_B,
};

class UIObject {
    public:
        virtual ~UIObject() {}

        // Método virtual puro para garantir que todas as subclasses implementem Draw
        virtual void Draw() = 0;

        Vector2 pos;
        bool isClickable;
        bool isCursorOn;
        bool isBlinkable;
};

class SimpleText : public UIObject {
    public:
        SimpleText() = default;
        SimpleText(std::string text, int fontSize, Vector2 pos, bool b, bool c);

        void Draw() override;
        void Draw(Color color=LIGHTGRAY);

        std::string text;
        int fontSize;
        int textWidth;
};

class PopUpMessage : public SimpleText {
    public:
        float timeLeft; 
        float duration;

        PopUpMessage(const std::string& text, Vector2 position, float duration = 5.0f)
        : SimpleText(text, TEXTBOX_FONTSIZE + 10, position, false, false), timeLeft(duration), duration(duration) {}

    void Update();
    bool IsExpired() const;

    void Draw(Color color);
};

class Box : public UIObject {
    public:
        Rectangle GetBox();
        BoxID GetID();
        void SetIsCursorOn(bool b);

        bool isPressed();
        void SetPressed(bool b);
        void TogglePressed();

        BoxID id;
        int width;
        int height;
        bool pressed;
};

class TextBox : public Box {
    public:
        TextBox(BoxID id, std::vector<std::string> strings, Vector2 pos, bool b, bool c);

        void SetNextText();
        void SetCurrentText(int index);
        std::string& GetText();
        void SetIsCursorOn(bool b);
        void Draw();

    protected:
        std::vector<SimpleText> texts;
        int currentTextIndex;
};

class BattleTextBox : public TextBox {
    public:
        BattleTextBox(BoxID id, std::vector<std::string> strings, Vector2 pos, bool b, bool c, EventID eventID, int8_t value);

        EventID GetEventID();
        int8_t& GetValue();
        void Draw();

    private:
        EventID eventID;
        int8_t value;
};

class PromptBox : public Box {
    public:
        PromptBox(Vector2 pos, bool b, bool c);

        void UpdateText(int key);
        std::string GetCurrentText();
        void ResetText();
        void SetIsCursorOn(bool b);
        void Draw();

    private:
        std::string currentText;
        int maxCharacters;

};

void PrintStartupsCount(int total);
void PrintEventDescription(EventID id);
void PrintAllStartupsInfo(std::vector<std::tuple<Startup, uint16_t>> startups);
void PrintBattles(Tournament t);
void PrintCurrentBattleAndPoints(Tournament t);
void PrintChampionStartup(Tournament t);
void PrintAllResults(Tournament t);
void PrintSpecialRoundInfo(Tournament t);
void PrintAllResultsWithEvents(Tournament t);

#endif