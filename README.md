# Soldering Reflow Hot Plate Controller

| Property          | Value                                                             |
|-------------------|-------------------------------------------------------------------|
| Project ID        | TJ28                                                              |
| Project Type      | Prototype, MVP                                                    |
| Initiated On      | 11th October 2026                                                 |
| Stack             | C/C++, PlatformIO, Arduino                                        |
| Keywords          | UNO, 16x02 Shield, PTC Heater, MAX6675, K-Type Thermocouple, SSR, 12V SMPS |
| Status            | Testing in progress                                               |

## 🚀 Overview

A feature-rich, menu-driven DIY reflow soldering controller built around an Arduino Uno, a 1602 LCD keypad shield, a PTC heating plate, and a PID control algorithm. It is designed for precise surface-mount assembly with thermal safety features and active cooling.

## 🌟 Features

- Multi-profile support: pre-configured and editable profiles for Low-Temp (BiSn 138°C), Leaded (Sn63/Pb37 183°C), and Lead-Free (SAC305 217°C) solders.

- PID thermal control: uses the PID_v1 library with time-proportioned solid-state relay (SSR) switching to reduce temperature overshoot.

- Interactive HMI: 16x2 LCD display paired with a 5-way tactile keypad shield for standalone menu navigation.

- Active cooling: fan-driven cool-down cycle with a safety cutoff at ≤ 40 °C.

- EEPROM persistence: custom profiles are stored on the onboard EEPROM.

## 🧩 Default Home Screen & Real-Time Telemetry

When the machine is plugged in and idle or actively running a profile, the default home screen continuously shows live vitals across the two 16-character lines:

```text
[Row 1] T: 24.5C  S: Idle
[Row 2] Tgt: Off  [UP=Menu]
```

### Why These Indicators Matter

| Indicator | Meaning                                             |
|-----------|-----------------------------------------------------|
| T:        | Current measured temperature (°C)                   |
| S:        | Current state: Idle, Preheat, Soak, Peak, or Cool   |
| Tgt:      | The target temperature (predefined setpoint)        |

### How They Change Dynamically

- Both values are tied directly to variables in the state machine and control loop.

- State (S:): an enum or variable (for example, `currentState`) is updated when thresholds are crossed. For example, when the thermocouple reaches the soak threshold, the state may change to `SOAK`, and the LCD displays `S:Soak`.

- Target (Tgt:): the reflow profile timer calculates the desired temperature each second, updates `targetTemperature`, and the loop prints the new value to Row 2 (for example, `Tgt:Off` → `Tgt:160C`).

## 🧭 UI Menu Structure & Flow

```text
[HOME SCREEN] (T: 24.5C S: Idle)
       │
       ├─(Press UP / DOWN)
       ▼
 [MAIN MENU] ◄───────────────────────────────────────────┐
       ├─> 1. Run Profile ──► [PROFILE SELECTOR]         │
       │                         ├─ 1. Low-Temp (138C)   │
       │                         ├─ 2. Leaded (183C)     │
       │                         ├─ 3. Lead-Free (217C)  │
       │                         ├─ 4. Custom            │
       │                         └─ (Press LEFT to Back)─┘
       │
       ├─> 2. Manual Heater ──► Set Target Temp & Auto-Off Timer
       │
       ├─> 3. Edit Profiles ──► Customize Soak/Peak Temps & Times (EEPROM Saved)
       │
       ├─> 4. Heater Off ────► Triggers Active Cool-Down (Fan ON until ≤ 40°C)
       │
       └─> 5. About ────────► Firmware Version & Other Info
```

### Navigation

| Button      | Action                                   |
|-------------|------------------------------------------|
| UP / DOWN   | Scroll menus; increment/decrement values |
| LEFT        | Back / cancel                            |
| RIGHT       | Select / confirm                         |
| SELECT      | Enter menu / commit value                |

## 📈 Supported Heater Profiles

### Reflow Run Profile

| Profile           | Soak Temp (°C) | Soak Time (s) | Peak Temp (°C) | Peak Time (s) |
|-------------------|----------------|---------------|----------------|---------------|
| Low-Temp (138°C)  | 100            | 60            | 155            | 45            |
| Leaded (183°C)    | 140            | 90            | 205            | 60            |
| Lead-Free (217°C) | 160            | 90            | 240            | 60            |

### Manual Control

| Options             | Range         |
|---------------------|---------------|
| Set Temperature     | ≤ 260°C       |
| Auto Off in (Timer) | HH:MM         |

### Safety Heater Off

| Options          | Range    |
|------------------|----------|
| Active Cool-Down | ≤ 40°C   |

## 📦 Bill of Materials (BOM)

| Component          | Description                                      |
|--------------------|--------------------------------------------------|
| Microcontroller    | Arduino UNO (ATmega328P)                         |
| User Interface     | 1602 LCD Board Keypad Shield                     |
| Heating Element    | 400W aluminum PTC heating plate, ~260°C max      |
| Temperature Sensor | MAX6675 SPI module with K-type thermocouple      |
| Power Switching    | Solid State Relay (SSR-40DA)                     |
| Cooling            | DC exhaust fan & BJT/N-channel MOSFET            |
| Power Supply       | 12V SMPS                                         |

## 🔌 Dependencies

Declared in `platformio.ini`:

- `PID_v1` → PID control loop
- `MAX6675` → thermocouple interface
- `LiquidCrystal` → 1602 LCD driver
- `EEPROM` → profile persistence

## 🧪 Usage

1. Power on the controller; the home screen shows the current temperature and `S: Idle`.

2. Press `UP` or `DOWN` to enter the main menu.

3. Choose `Run Profile`, then select Low-Temp, Leaded, Lead-Free, or Custom.

4. The controller automatically walks through `Preheat → Soak → Peak → Cool`.

5. When finished, the fan runs until the plate drops to ≤ 40 °C.

6. To abort at any time, choose `Heater Off` from the main menu.

## 🛡️ Safety Notes

- The PTC plate can reach temperatures high enough to cause severe burns and ignite flammable materials. Keep the work area clear.

- Always use a properly rated SSR and heatsink, and do not exceed the plate's 260 °C limit.

- Never operate the controller unattended during early testing.

- Verify thermocouple readings against a known reference before trusting the profile.

## 🙏 Acknowledgements

- Arduino community and the maintainers of `PID_v1`, `Adafruit-MAX6675`, and `LiquidCrystal`.

- Everyone who has shared open-source reflow controller designs—this project stands on their shoulders.
