#include "global.h"
#include <LiquidCrystal.h>

// LCD Pin Mapping (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(
    LCD_RS,
    LCD_EN,
    LCD_D4,
    LCD_D5,
    LCD_D6,
    LCD_D7
);

// Corresponding string labels for LCD
const char* machineStateStr[] = {
    "Idle",
    "Heat",   // Preheat
    "Soak",
    "Refl",   // Reflow
    "Cool",   // Cooling
    "Man"     // Manual
};

// Menu variables
int menuIndex = 0;
const int totalMenuItems = 5;
const char* mainMenuStr[] = {
  "1. Run Profile",
  "2. Manual Control",
  "3. Edit Profiles",
  "4. Heater Off",
  "5. About"
};

// Start in the home screen state
UIState currentUI = UI_HOME;

// Track the current active machine state
MachineState currentState = STATE_IDLE;

// Telemetry variables
int targetTemp = 0;
float currentTemp = 0.0;

// Read buttons via analog pin
Button readLCDButton() {
    int adcKeyIn = analogRead(IO_KEYS);
    if (adcKeyIn > 1000) return BTN_NONE;
    if (adcKeyIn < 50)   return BTN_RIGHT;
    if (adcKeyIn < 195)  return BTN_UP;
    if (adcKeyIn < 380)  return BTN_DOWN;
    if (adcKeyIn < 555)  return BTN_LEFT;
    if (adcKeyIn < 850)  return BTN_SELECT;
    return BTN_NONE;
}

void updateHomeScreen() {
    char tempBuf[10] = {'\0'};
    char stateBuf[7] = {'\0'};

    // Format temperature (e.g., "T:23.5°C" or "T:230.5°C")
    snprintf(tempBuf, sizeof(tempBuf), "T:%.1f%cC", (double)currentTemp, (char)223);

    // Format state to ensure it fits tightly within 6 characters (e.g., "S:Idle")
    snprintf(stateBuf, sizeof(stateBuf), "S:%s", machineStateStr[currentState]);

    // Row 1: Print temperature
    lcd.setCursor(0, 0);
    lcd.print(tempBuf);

    // Clear any ghost characters between the temp and the state slot
    for (int i = strlen(tempBuf); i < 10; i++) {
        lcd.print(" ");
    }

    // State into the last 6 characters (Column 10)
    lcd.setCursor(10, 0);
    lcd.print(stateBuf);

    // Row 2: Target & Menu Prompt
    char row2Buf[17];
    if (targetTemp == 0) {
        snprintf(row2Buf, sizeof(row2Buf), "Tgt:Off   [UP=M]");

    } else {
        snprintf(row2Buf, sizeof(row2Buf), "Tgt:%d%cC [UP=M]", targetTemp, (char)223);
    }

    lcd.setCursor(0, 1);
    lcd.print(row2Buf);
    for (int i = strlen(row2Buf); i < 16; i++) {
        lcd.print(" ");
    }
}

void updateMainMenu() {
  char buf[17];

  lcd.setCursor(0, 0);
  lcd.print("--- MAIN MENU ---");

  // Format menu item with buffer padding to clear trailing ghost characters
  snprintf(buf, sizeof(buf), ">%s", mainMenuStr[menuIndex]);
  lcd.setCursor(0, 1);
  lcd.print(buf);
  for (int i = strlen(buf); i < 16; i++) {
    lcd.print(" ");
  }
}

void setup() {
    pinMode(IO_KEYS, INPUT);

    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reflow Controller");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");
    delay(1500);
    lcd.clear();
}

void loop() {
  Button btn = readLCDButton();

  if (btn != BTN_NONE) {
    delay(200); // Debounce delay

    switch (currentUI) {
        case UI_HOME:
            if (btn == BTN_UP || btn == BTN_DOWN) {
                currentUI = UI_MAIN_MENU;
                menuIndex = 0;
                lcd.clear();
            }
            break;

        case UI_MAIN_MENU:
            if (btn == BTN_UP) {
                menuIndex = (menuIndex - 1 + totalMenuItems) % totalMenuItems;

            } else if (btn == BTN_DOWN) {
                menuIndex = (menuIndex + 1) % totalMenuItems;

            } else if (btn == BTN_LEFT) {
                currentUI = UI_HOME;
                lcd.clear();

            } else if (btn == BTN_SELECT) {

                // Enter selected submenu
                switch (menuIndex) {
                    case 0: currentUI = UI_RUN_PROFILE; break;
                    case 1: currentUI = UI_MANUAL_CTRL; break;
                    case 2: currentUI = UI_EDIT_PROFILES; break;
                    case 3: currentUI = UI_COOL_DOWN; break;
                    case 4: currentUI = UI_ABOUT; break;
                }
                lcd.clear();
            }
            break;

        case UI_RUN_PROFILE:
        case UI_MANUAL_CTRL:
        case UI_EDIT_PROFILES:
        case UI_COOL_DOWN:
        case UI_ABOUT:
            if (btn == BTN_LEFT) {
                currentUI = UI_MAIN_MENU;
                lcd.clear();
            }
            break;
        }
    }

    // Refresh display based on active state
    if (currentUI == UI_HOME) {
        updateHomeScreen();

    } else if (currentUI == UI_MAIN_MENU) {
        updateMainMenu();

    } else {
        // Submenu placeholder screens
        lcd.setCursor(0, 0);
        lcd.print(mainMenuStr[menuIndex]);
        lcd.setCursor(0, 1);
        lcd.print("[LEFT=Back]     ");
    }

    delay(100);
}
