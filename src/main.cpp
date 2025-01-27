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

void setup()
{
    Serial.begin(9600);
    initializeButtons();
    _state = DISPLAY_1;

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (true)
            ;
    }
}

void loop()
{
    updateButtonStates();

    if (buttons[0].btn.isPressed())
        _state = static_cast<DisplayState>((_state + 1) % NUM_STATES);
    if (buttons[2].btn.isPressed())
        _state = static_cast<DisplayState>((_state - 1 + NUM_STATES) % NUM_STATES);
    handleStateChange();
    debugButtons();
    delay(10);
}

void initializeButtons()
{
    for (int i = 0; i < BTN_NUM; i++)
    {
        buttons[i].btn.setDebounceTime(DEBOUNCE_TIME);
    }
}

void updateButtonStates()
{
    for (int i = 0; i < BTN_NUM; i++)
    {
        buttons[i].btn.loop();
    }
}

void handleStateChange()
{
    displayFunctions[_state]();
}

void (*displayFunctions[3])() = {set_display_1, set_display_2, set_display_3};

void set_display_1()
{
    const char *messages[] = {"Temp&Hum", "Temp : 24C", "Hum : 70%", "temp"};
    updateDisplay(messages, 3);
}

void set_display_2()
{
    const char *messages[] = {"Light Intnsty", "30%", "Pretty bright", "light"};
    updateDisplay(messages, 3);
}

void set_display_3()
{
    const char *messages[] = {"Rayhan K", "rayhank.com", "ESP32 Monitor", "conf"};
    updateDisplay(messages, 3);
}

void updateDisplay(const char *message[], int size)
{
    display.clearDisplay();

    if (message[3] == "temp")
    {
        display.fillRect(0, 10, 10, 40, SSD1306_WHITE);
        display.fillCircle(5, 50, 5, SSD1306_WHITE);
        display.drawLine(5, 10, 5, 20, SSD1306_BLACK);
        display.drawCircle(5, 50, 3, SSD1306_BLACK);
    }
    else if (message[3] == "light") {
        display.drawCircle(10, 30, 8, SSD1306_WHITE);  
        for (int angle = 0; angle < 360; angle += 45) {  
            int xStart = 10 + cos(radians(angle)) * 10;
            int yStart = 30 + sin(radians(angle)) * 10;
            int xEnd = 10 + cos(radians(angle)) * 14;
            int yEnd = 30 + sin(radians(angle)) * 14;
            display.drawLine(xStart, yStart, xEnd, yEnd, SSD1306_WHITE);
        }
    }
    else if (message[3] == "conf") {
       display.drawCircle(10, 30, 8, SSD1306_WHITE);  
        for (int angle = 0; angle < 360; angle += 60) {  
            int xOuter = 10 + cos(radians(angle)) * 12;
            int yOuter = 30 + sin(radians(angle)) * 12;
            int xInner = 10 + cos(radians(angle + 30)) * 8;
            int yInner = 30 + sin(radians(angle + 30)) * 8;
            display.drawLine(xOuter, yOuter, xInner, yInner, SSD1306_WHITE);
        }
    }

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    for (int i = 0; i < size; i++)
    {
        display.setCursor(30, 10 + (i * 10)); // Sesuaikan posisi teks agar sejajar dengan ikon
        display.println(message[i]);
    }

    // Tampilkan semua ke layar
    display.display();
}

void debugButtons()
{
    for (int i = 0; i < BTN_NUM; i++)
    {
        if (buttons[i].btn.isPressed())
        {
            Serial.printf("Button %d pressed\n", i + 1);
        }
    }
    Serial.printf("Current state: %d\n", _state);
}
