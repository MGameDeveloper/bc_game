#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../inc/bc_key.h"
#include "../inc/bc_log.h"

static ekey key_map[GLFW_KEY_LAST];

bool bc_key::init()
{
	static bool keys_initialized = false;
	if (!keys_initialized)
	{
		key_map[GLFW_KEY_A] = ekey_A;
		key_map[GLFW_KEY_B] = ekey_B;
		key_map[GLFW_KEY_C] = ekey_C;
		key_map[GLFW_KEY_D] = ekey_D;
		key_map[GLFW_KEY_E] = ekey_E;
		key_map[GLFW_KEY_F] = ekey_F;
		key_map[GLFW_KEY_G] = ekey_G;
		key_map[GLFW_KEY_H] = ekey_H;
		key_map[GLFW_KEY_I] = ekey_I;
		key_map[GLFW_KEY_J] = ekey_J;
		key_map[GLFW_KEY_K] = ekey_K;
		key_map[GLFW_KEY_L] = ekey_L;
		key_map[GLFW_KEY_M] = ekey_M;
		key_map[GLFW_KEY_N] = ekey_N;
		key_map[GLFW_KEY_O] = ekey_O;
		key_map[GLFW_KEY_P] = ekey_P;
		key_map[GLFW_KEY_Q] = ekey_Q;
		key_map[GLFW_KEY_R] = ekey_R;
		key_map[GLFW_KEY_S] = ekey_S;
		key_map[GLFW_KEY_T] = ekey_T;
		key_map[GLFW_KEY_U] = ekey_U;
		key_map[GLFW_KEY_V] = ekey_V;
		key_map[GLFW_KEY_W] = ekey_W;
		key_map[GLFW_KEY_X] = ekey_X;
		key_map[GLFW_KEY_Y] = ekey_Y;
		key_map[GLFW_KEY_Z] = ekey_Z;

		key_map[GLFW_KEY_0] = ekey_Num0;
		key_map[GLFW_KEY_1] = ekey_Num1;
		key_map[GLFW_KEY_2] = ekey_Num2;
		key_map[GLFW_KEY_3] = ekey_Num3;
		key_map[GLFW_KEY_4] = ekey_Num4;
		key_map[GLFW_KEY_5] = ekey_Num5;
		key_map[GLFW_KEY_6] = ekey_Num6;
		key_map[GLFW_KEY_7] = ekey_Num7;
		key_map[GLFW_KEY_8] = ekey_Num8;
		key_map[GLFW_KEY_9] = ekey_Num9;

		key_map[GLFW_KEY_KP_0] = ekey_Numpad0;
		key_map[GLFW_KEY_KP_1] = ekey_Numpad1;
		key_map[GLFW_KEY_KP_2] = ekey_Numpad2;
		key_map[GLFW_KEY_KP_3] = ekey_Numpad3;
		key_map[GLFW_KEY_KP_4] = ekey_Numpad4;
		key_map[GLFW_KEY_KP_5] = ekey_Numpad5;
		key_map[GLFW_KEY_KP_6] = ekey_Numpad6;
		key_map[GLFW_KEY_KP_7] = ekey_Numpad7;
		key_map[GLFW_KEY_KP_8] = ekey_Numpad8;
		key_map[GLFW_KEY_KP_9] = ekey_Numpad9;

		key_map[GLFW_KEY_F1]  = ekey_F1;
		key_map[GLFW_KEY_F2]  = ekey_F2;
		key_map[GLFW_KEY_F3]  = ekey_F3;
		key_map[GLFW_KEY_F4]  = ekey_F4;
		key_map[GLFW_KEY_F5]  = ekey_F5;
		key_map[GLFW_KEY_F6]  = ekey_F6;
		key_map[GLFW_KEY_F7]  = ekey_F7;
		key_map[GLFW_KEY_F8]  = ekey_F8;
		key_map[GLFW_KEY_F9]  = ekey_F9;
		key_map[GLFW_KEY_F10] = ekey_F10;
		key_map[GLFW_KEY_F11] = ekey_F11;
		key_map[GLFW_KEY_F12] = ekey_F12;

		key_map[GLFW_KEY_ESCAPE]       = ekey_Escape;
		key_map[GLFW_KEY_TAB]          = ekey_Tab;
		key_map[GLFW_KEY_CAPS_LOCK]    = ekey_Capslock;
		key_map[GLFW_KEY_SPACE]        = ekey_Space;
		key_map[GLFW_KEY_BACKSPACE]    = ekey_Backspace;
		key_map[GLFW_KEY_ENTER]        = ekey_Enter;
		key_map[GLFW_KEY_PRINT_SCREEN] = ekey_Printscreen;
		key_map[GLFW_KEY_INSERT]       = ekey_Insert;
		key_map[GLFW_KEY_DELETE]       = ekey_Delete;
		key_map[GLFW_KEY_HOME]         = ekey_Home;
		key_map[GLFW_KEY_END]          = ekey_End;
		key_map[GLFW_KEY_NUM_LOCK]     = ekey_Numlock;
		key_map[GLFW_KEY_KP_DIVIDE]    = ekey_Divide;
		key_map[GLFW_KEY_KP_MULTIPLY]  = ekey_Multiply;
		key_map[GLFW_KEY_KP_SUBTRACT]  = ekey_Subtract;
		key_map[GLFW_KEY_KP_ADD]       = ekey_Add;
		key_map[GLFW_KEY_KP_DECIMAL]   = ekey_Decimal;
		key_map[GLFW_KEY_PERIOD]       = ekey_Period;
		key_map[GLFW_KEY_SLASH]        = ekey_Slash;
		key_map[GLFW_KEY_BACKSLASH]    = ekey_Backslash;
		key_map[GLFW_KEY_COMMA]        = ekey_Comma;
		key_map[GLFW_KEY_SEMICOLON]    = ekey_Semicolon;
		//ekey::map[] = ekey::tilda;
		//ekey::map[] = ekey::colon;

		key_map[GLFW_KEY_UP]    = ekey_Up;
		key_map[GLFW_KEY_DOWN]  = ekey_Down;
		key_map[GLFW_KEY_LEFT]  = ekey_Left;
		key_map[GLFW_KEY_RIGHT] = ekey_Right;

		keys_initialized = true;

		bc_log::info("bc_key initiated");
	}
}

ekey bc_key::get_key(i32 key)
{
	if (key < 0 || key > GLFW_KEY_LAST)
	{
		bc_log::warning("[ bc_key::get_key( %d ) ]: invalid param");
		return ekey_Unknown;
	}

	return key_map[key];
}

ekeymod bc_key::get_keymod(i32 mod)
{
	i32 m = ekeymod_Unknown;
	if (mod & GLFW_MOD_SHIFT)
		m |= (i32)ekeymod_Shift;
	if (mod & GLFW_MOD_CONTROL)
		m |= (i32)ekeymod_Ctrl;
	if (mod & GLFW_MOD_ALT)
		m |= (i32)ekeymod_Alt;

	return ekeymod(m);
}