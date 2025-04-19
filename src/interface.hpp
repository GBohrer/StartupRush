#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>
#include <raylib.h>

#include "classes.hpp"

#define SCREEN_WIDTH                1280
#define SCREEN_HEIGHT               720

#define TEXTBOX_WIDTH               200
#define TEXTBOX_HEIGHT              50
#define TEXTBOX_FONTSIZE            25
#define TEXTBOX_FONTSIZE_2          40
#define BOX_THICKNESS               5
#define TITLE_FONTSIZE              80
#define PROMPTBOX_PADDING           15
#define BOX_OFFSET                  70

#define COLOR_BACKGROUND            (Color){10, 10, 40, 255}
#define COLOR_MOUSE_HOVER           (Color){ 50, 240, 48, 255}

const Vector2 SCREEN_POS_CENTER_1            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_LEFT_1       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_RIGHT_1      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 300};
const Vector2 SCREEN_POS_CENTER_2            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 230};
const Vector2 SCREEN_POS_CENTER_LEFT_2       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 230};
const Vector2 SCREEN_POS_CENTER_RIGHT_2      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 230};
const Vector2 SCREEN_POS_CENTER_3            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 160};
const Vector2 SCREEN_POS_CENTER_LEFT_3       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 160};
const Vector2 SCREEN_POS_CENTER_RIGHT_3      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 160};
const Vector2 SCREEN_POS_CENTER_4            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 90};
const Vector2 SCREEN_POS_CENTER_LEFT_4       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 90};
const Vector2 SCREEN_POS_CENTER_RIGHT_4      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 90};
const Vector2 SCREEN_POS_CENTER_5            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 20};
const Vector2 SCREEN_POS_CENTER_LEFT_5       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 20};
const Vector2 SCREEN_POS_CENTER_RIGHT_5      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 - 20};
const Vector2 SCREEN_POS_CENTER_6            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 50};
const Vector2 SCREEN_POS_CENTER_LEFT_6       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 50};
const Vector2 SCREEN_POS_CENTER_RIGHT_6      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 + 50};
const Vector2 SCREEN_POS_CENTER_7            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 120};
const Vector2 SCREEN_POS_CENTER_LEFT_7       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 120};
const Vector2 SCREEN_POS_CENTER_RIGHT_7      = {(float)SCREEN_WIDTH / 2 + 500, (float)SCREEN_HEIGHT / 2 + 120};
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
    VALIDATE1,
    VALIDATE2,
    VALIDATE3,
    VALIDATE4,
    NEW_STARTUP,
    CREATE,
    EXIT,
    BACK,
    YES,
    NO,
    PROMPT,
    CONTENT,
    GOODPITCH_A,
    BUGS_A,
    USERS_A,
    ANGRY_A,
    FAKENEWS_A,
    GOODPITCH_B,
    BUGS_B,
    USERS_B,
    ANGRY_B,
    FAKENEWS_B
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

        void Draw();

        std::string text;
        int fontSize;
        int textWidth;
};

class PopUpMessage : public SimpleText {
    public:
        float timeLeft; 
        float duration;

        PopUpMessage(const std::string& text, Vector2 position, float duration = 3.0f)
        : SimpleText(text, TEXTBOX_FONTSIZE + 10, position, false, false), timeLeft(duration), duration(duration) {}

    void Update() {
        timeLeft -= GetFrameTime();
    }

    bool IsExpired() const {
        return timeLeft <= 0;
    }

    void Draw() override {
        SimpleText::Draw();
    }
};

class Box : public UIObject {
    public:
        Rectangle GetBox();
        BoxID GetID();
        void SetIsCursorOn(bool b);

        BoxID id;
        int width;
        int height;
};

class TextBox : public Box {
    public:
        TextBox(BoxID id, std::vector<std::string> strings, Vector2 pos, bool b, bool c);

        void SetNextText();
        std::string& GetText();
        void SetIsCursorOn(bool b);
        void Draw();

    private:
        std::vector<SimpleText> texts;
        int currentTextIndex;
};

class BattleTextBox : public TextBox {
    public:
        BattleTextBox(BoxID id, std::vector<std::string> strings, Vector2 pos, bool b, bool c, EventID eventID, int8_t value);

        EventID GetEventID();
        int8_t GetValue();

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
void PrintAllStartupsInfo(std::vector<std::tuple<Startup, uint16_t>> startups);
void PrintBattles(Tournament t);
void PrintCurrentBattleAndPoints(Tournament t);

#endif