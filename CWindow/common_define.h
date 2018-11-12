#pragma once

//#define SHORTCUT_TEST

#define DELETE(p)       do { if (NULL != p) { delete p; p = NULL; } } while(0);

// size
#define FIXED_TITLEBAR_HEIGHT       45
#define FIXED_MENUBAR_HEIGHT        45
#define FIXED_STATUSBAR_HEIGHT      60

// index
#define TITLEBAR_INDEX              0
#define MENUBAR_INDEX               1
#define CENTERWIDGET_INDEX          2
#define STATUSBAR_INDEX             3

// qss
const char* const kszQssWindows     = ":/qss/Resources/qss/win.qss";

// object name
const char* const kszWindowTitle    = "labelWindowTitle";
const char* const kszWindowMaximize = "btnWindowMax";
const char* const kszWindowMinimize = "btnWindowMin";
const char* const kszWindowExit     = "btnWindowExit";
