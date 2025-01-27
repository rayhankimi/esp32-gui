#include <Arduino.h>
#include <ezButton.h>
#include <button.h>


#define BTN_NUM 3
#define BTN_1 15
#define BTN_2 2
#define BTN_3 4

int _state;

enum OLedDisplay {
    DISPLAY_1,
    DISPLAY_2,
    DISPLAY_3,
};

struct Button {
    uint8_t pin;
    ezButton btn;
    uint8_t state;
};

Button buttons[BTN_NUM] = {
    {BTN_1, ezButton(BTN_1), 0},
    {BTN_2, ezButton(BTN_2), 0},
    {BTN_3, ezButton(BTN_3), 0},
};

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < BTN_NUM; i++) {
        buttons[i].btn.setDebounceTime(50);
    }
}

void loop() {

    // Initialize buttons
    for (int i = 0; i < BTN_NUM; i++) {
        buttons[i].btn.loop();
        if (buttons[i].btn.isPressed()) {
            buttons[i].state = 1;
        } else if (buttons[i].btn.isReleased()) {
            buttons[i].state = 0;
        }
    }

    buttons[0].state ? _state++ : _state;
    buttons[2].state ? _state-- : _state;


    // Debug button here
    buttons[0].state ? Serial.println("Button 1 pressed") : Serial.println("");
    buttons[1].state ? Serial.println("Button 2 pressed") : Serial.println("");
    buttons[2].state ? Serial.println("Button 3 pressed") : Serial.println("");
    Serial.println(_state);

    delay(100);
}

// put function definitions here:
