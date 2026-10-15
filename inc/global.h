#ifndef REFLOW_GLOBALS_H
#define REFLOW_GLOBALS_H

#include <Arduino.h>

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
    int soakTemp;
    unsigned long soakTimeMs;
    int peakTemp;
    unsigned long peakTimeMs;
};

#endif
