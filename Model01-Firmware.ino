
// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif


#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Numlock.h"
#include "Kaleidoscope.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Chase.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LED-AlphaSquare.h"
#include "Kaleidoscope-Heatmap.h"
#include "Kaleidoscope-Model01-TestMode.h"

#define MACRO_VERSION_INFO 1
#define Macro_VersionInfo M(MACRO_VERSION_INFO)
#define MACRO_ANY 2
#define Macro_Any M(MACRO_ANY)
#define NUMPAD_KEYMAP 2

#define GENERIC_DVORAK_FN2  KEYMAP_STACKED ( \
___,      Key_F1,           Key_F2,      Key_F3,        Key_F4,           Key_F5,             XXX,         \
___,     ___,      Key_mouseUp,         ___, Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE, \
___, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,                   \
___,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  ___, \
            Key_LeftControl, ___, ___, ___,  \
                                                                ___,   \
\
Consumer_ScanPreviousTrack,                 Key_F6,                   Key_F7,                    Key_F8,           Key_F9,          Key_F10,          Key_F11, \
Key_Backslash,              Consumer_ScanNextTrack,     Key_LeftCurlyBracket,     Key_RightCurlyBracket,  Key_LeftBracket, Key_RightBracket,          Key_F12, \
                                     Key_LeftArrow,            Key_DownArrow,               Key_UpArrow,   Key_RightArrow,              ___,         Key_Pipe, \
___,                             Key_Menu, Consumer_VolumeDecrement,  Consumer_VolumeIncrement,              ___,    Key_Backslash,   Key_RightShift, \
___, ___, ___,  ___, \
___ \
)



#define NUMPAD KEYMAP_STACKED  (\
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___,       \
    ___, ___, ___, ___, ___, ___, ___,  \
               ___, ___, ___, ___,  \
                 Key_Keymap1_Momentary, \
\
\
    Macro_VersionInfo,  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___, \
    ___, ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___, \
         ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote, \
    ___, ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter, \
    ___, ___, ___, ___, \
    Key_Keymap1_Momentary \
)

#define DVORAK KEYMAP_STACKED ( \
    ___,           Key_1,         Key_2,     Key_3,        Key_4, Key_5, Key_LEDEffectNext, \
    Key_Tab,       Key_Semicolon, Key_Comma, Key_Period,   Key_P, Key_Y, Key_Backtick,      \
    Key_LeftAlt,   Key_A,         Key_O,     Key_E,        Key_U, Key_I,                    \
    Key_LeftShift, Key_Quote,     Key_Q,     Key_J,        Key_K, Key_X, Key_Escape,        \
            Key_LeftControl, Key_Backspace, Key_Delete, Key_Home,         \
                          Key_Keymap1_Momentary,     \
\
    Macro_Any,     Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_ToggleNumlock, \
    Key_Slash,     Key_F, Key_G, Key_C,     Key_R,      Key_L,         Key_Equals,       \
                   Key_D, Key_H, Key_T,     Key_N,      Key_S,         Key_Minus,       \
    Key_RightGui,  Key_B, Key_M, Key_W,     Key_V,      Key_Z,         Key_RightShift,       \
            Key_End, Key_Enter, Key_Spacebar, Key_RightControl,                   \
    Key_Keymap1_Momentary \
)

const Key keymaps[][ROWS][COLS] PROGMEM = {
  DVORAK,
  GENERIC_DVORAK_FN2,
  NUMPAD
};

static LEDSolidColor solidRed(160, 0, 0);
static LEDSolidColor solidViolet(130, 0, 120);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == TOGGLENUMLOCK && keyToggledOn(keyState)) {
    return NumLock.toggle();
  } else if (macroIndex == 1 && keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  } else if (macroIndex == MACRO_ANY) {
    static Key lastKey;
    if (keyToggledOn(keyState))
      lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);

    if (keyIsPressed(keyState))
      kaleidoscope::hid::pressKey(lastKey);
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup(KEYMAP_SIZE);
  BootKeyboard.begin();
  Kaleidoscope.use(&TestMode,
                   &LEDControl, &LEDOff,
                   &LEDRainbowEffect, &LEDRainbowWaveEffect, &LEDChaseEffect,
                   &solidRed, &solidViolet,
                   &LEDBreatheEffect,
                   &AlphaSquareEffect,
                   &StalkerEffect,
				           &HeatmapEffect,
                   &NumLock,
                   &Macros,
                   &MouseKeys,
                   NULL);

  NumLock.numPadLayer = NUMPAD_KEYMAP;
  AlphaSquare.color = { 255, 0, 0 };
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  StalkerEffect.variant = STALKER(BlazingTrail);
  LEDOff.activate();
}


void loop() {
  Kaleidoscope.loop();
}
