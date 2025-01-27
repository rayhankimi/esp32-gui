#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
#include <ezButton.h>

enum DisplayState
{
    DISPLAY_1,
    DISPLAY_2,
    DISPLAY_3,
    NUM_STATES,
};

void setup();
void loop();

void updateButtonStates();
void handleStateChange();
void debugButtons();
void updateDisplay(const char* messages[], int size);
void initializeButtons();
void set_display_1();
void set_display_2();
void set_display_3();

// Global function pointer array
extern void (*displayFunctions[3])();

#endif
