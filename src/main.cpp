#include <Arduino.h>
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <gui.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BTN_NUM 3
#define BTN_1 15
#define BTN_2 2
#define BTN_3 4
#define DEBOUNCE_TIME 69

struct Button
{
    uint8_t pin;
    ezButton btn;
};

Button buttons[BTN_NUM] = {
    {BTN_1, ezButton(BTN_1)},
    {BTN_2, ezButton(BTN_2)},
    {BTN_3, ezButton(BTN_3)},
};

DisplayState _state;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
    Serial.begin(9600);
    initializeButtons();
    _state = DISPLAY_1;

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true);
    }
}

void loop() {
    updateButtonStates();

    if (buttons[0].btn.isPressed()) _state = static_cast<DisplayState>((_state + 1) % NUM_STATES);
    if (buttons[2].btn.isPressed()) _state = static_cast<DisplayState>((_state - 1 + NUM_STATES) % NUM_STATES);

    handleStateChange();
    debugButtons();
    delay(10);
}

void initializeButtons() {
    for (int i = 0; i < BTN_NUM; i++) {
        buttons[i].btn.setDebounceTime(DEBOUNCE_TIME);
    }
}

void updateButtonStates() {
    for (int i = 0; i < BTN_NUM; i++) {
        buttons[i].btn.loop();
    }
}

void handleStateChange() {
    displayFunctions[_state]();
}

void (*displayFunctions[3])() = {set_display_1, set_display_2, set_display_3};
void set_display_1() { updateDisplay("Display 1"); }
void set_display_2() { updateDisplay("Display 2"); }
void set_display_3() { updateDisplay("Display 3"); }

void updateDisplay(const char* message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(message);
    display.display();
}

void debugButtons() {
    for (int i = 0; i < BTN_NUM; i++) {
        if (buttons[i].btn.isPressed()) {
            Serial.printf("Button %d pressed\n", i + 1);
        }
    }
    Serial.printf("Current state: %d\n", _state);
}
