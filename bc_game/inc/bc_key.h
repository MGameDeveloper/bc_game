#pragma once

#include "bc_ints.h"

enum ekeystate
{
	ekeystate_Press,
	ekeystate_Release,
	ekeystate_Repeat
};

enum ekey
{
	ekey_Unknown = -1,

	ekey_Any,

	// Keyboard Buttons
	ekey_A, 
	ekey_B, 
	ekey_C, 
	ekey_D, 
	ekey_E,
	ekey_F,
	ekey_G,
	ekey_H,
	ekey_I,
	ekey_J,
	ekey_K,
	ekey_L,
	ekey_M,
	ekey_N,
	ekey_O,
	ekey_P,
	ekey_Q,
	ekey_R,
	ekey_S,
	ekey_T,
	ekey_U,
	ekey_V,
	ekey_W,
	ekey_X,
	ekey_Y,
	ekey_Z,

	ekey_Num0, 
	ekey_Num1,
	ekey_Num2,
	ekey_Num3,
	ekey_Num4,
	ekey_Num5,
	ekey_Num6,
	ekey_Num7,
	ekey_Num8,
	ekey_Num9,

	ekey_Numpad0, 
	ekey_Numpad1,
	ekey_Numpad2,
	ekey_Numpad3,
	ekey_Numpad4,
	ekey_Numpad5,
	ekey_Numpad6,
	ekey_Numpad7,
	ekey_Numpad8,
	ekey_Numpad9,

	ekey_F1, 
	ekey_F2,
	ekey_F3,
	ekey_F4,
	ekey_F5,
	ekey_F6,
	ekey_F7,
	ekey_F8,
	ekey_F9,
	ekey_F10,
	ekey_F11,
	ekey_F12,

	ekey_Escape,
	ekey_Tab,
	ekey_Capslock,
	ekey_Space, 
	ekey_Backspace,
	ekey_Enter,
	ekey_Printscreen,
	ekey_Insert,
	ekey_Delete,
	ekey_Home,
	ekey_End,
	ekey_Numlock,
	ekey_Divide,
	ekey_Multiply,
	ekey_Subtract,
	ekey_Add, 
	ekey_Decimal, 
	ekey_Period,
	ekey_Slash,
	ekey_Backslash, 
	ekey_Comma,
	ekey_Semicolon,
	ekey_Up,
	ekey_Down,
	ekey_Left,
	ekey_Right,

	// Mouse Buttons
	ekey_LMouse,
	ekey_RMouse,
	ekey_MMouse,
	ekey_MouseWheelUp,
	ekey_MouseWheelDown,

	// Gampad buttons
	ekey_Square,
	ekey_Cross,
	ekey_Circle, 
	ekey_Triangle, 
	ekey_DpadUp, 
	ekey_DpadDown, 
	ekey_DpadLeft, 
	ekey_DpadRight,
	ekey_L1, 
	ekey_L2, 
	ekey_L3,
	ekey_R1,
	ekey_R2,
	ekey_R3,
	ekey_Select,
	ekey_Start,
	ekey_LAnalgoUp, 
	ekey_LAnalogDown,
	ekey_LAnalogLeft,
	ekey_LAnalogRight,
	ekey_RAnalgoUp, 
	ekey_RAnalogDown, 
	ekey_RAnalogLeft, 
	ekey_RAnalogRight,

	Count
};

enum ekeymod
{
	ekeymod_Unknown = -1,
	ekeymod_Shift   = 1 << 0,
	ekeymod_Ctrl    = 1 << 1,
	ekeymod_Alt     = 1 << 2,
};

class bc_key
{
public:
	static bool init();
	static ekey get_key(i32 key);
	static ekeymod get_keymod(i32 mod);
};