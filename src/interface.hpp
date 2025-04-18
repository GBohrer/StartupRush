#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>

#define SCREEN_WIDTH                1280
#define SCREEN_HEIGHT               720

#define TEXTBOX_WIDTH               260
#define TEXTBOX_HEIGHT              70
#define TEXTBOX_FONTSIZE            40
#define BOX_THICKNESS               5
#define TITLE_FONTSIZE              80
#define PROMPTBOX_PADDING           15

#define COLOR_BACKGROUND            (Color){10, 10, 40, 255}
#define COLOR_MOUSE_HOVER           (Color){ 50, 240, 48, 255}

const Vector2 SCREEN_POS_CENTER_1            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - 100};
const Vector2 SCREEN_POS_CENTER_LEFT_1       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 - 100};
const Vector2 SCREEN_POS_CENTER_2            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_LEFT_2       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_3            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 100};
const Vector2 SCREEN_POS_CENTER_LEFT_3       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 100};
const Vector2 SCREEN_POS_CENTER_4            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 200};
const Vector2 SCREEN_POS_CENTER_LEFT_4       = {(float)SCREEN_WIDTH / 2 - 500, (float)SCREEN_HEIGHT / 2 + 200};
const Vector2 SCREEN_POS_CENTER_5            = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 300};
const Vector2 SCREEN_POS_CENTER_TOP          = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_CENTER              = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_CENTER_BOTTOM       = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_CENTER_BOTTOM_LEFT  = {(float)SCREEN_WIDTH / 2 - 300, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_CENTER_BOTTOM_RIGHT = {(float)SCREEN_WIDTH / 2 + 300, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_RIGHT_TOP           = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_RIGHT               = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_RIGHT_BOTTOM        = {(float)SCREEN_WIDTH / 2 + SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_LEFT_TOP            = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 3};
const Vector2 SCREEN_POS_LEFT                = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2};
const Vector2 SCREEN_POS_LEFT_BOTTOM         = {(float)SCREEN_WIDTH / 2 - SCREEN_WIDTH / 3, (float)SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3};


enum BoxID {
    BEGIN_TOURNAMENT,
    NEW_STARTUP,
    CREATE,
    EXIT,
    BACK,
    YES,
    NO,
    PROMPT,
    CONTENT
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
        : SimpleText(text, TEXTBOX_FONTSIZE - 10, position, false, false), timeLeft(duration), duration(duration) {}

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


#endif