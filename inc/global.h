#ifndef REFLOW_GLOBALS_H
#define REFLOW_GLOBALS_H

#include <Arduino.h>

// Hardware Pins
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_LED 10
#define IO_KEYS A0

#define IO_FAN 2
#define IO_SSR 3

#define TEMP_CK 13
#define TEMP_SO 12
#define TEMP_CS A1

// Button definitions
enum Button {
    BTN_NONE = 0,
    BTN_RIGHT,
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_SELECT
};

// System UI States
enum UIState {
    UI_HOME,
    UI_MAIN_MENU,
    UI_RUN_PROFILE,
    UI_RUNNING,
    UI_MANUAL_CTRL,
    UI_EDIT_PROFILES,
    UI_COOL_DOWN,
    UI_ABOUT
};

// Machine / Reflow Operational States for "S:"
enum MachineState {
    STATE_IDLE = 0,
    STATE_PREHEAT,
    STATE_SOAK,
    STATE_REFLOW,
    STATE_COOL,
    STATE_MANUAL
};

// Profile Structure Definition
struct ReflowProfile {
    const char* name;
    int soakTemp;
    unsigned long soakTimeMs;
    int peakTemp;
    unsigned long peakTimeMs;
};

#endif
