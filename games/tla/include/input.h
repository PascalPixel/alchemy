#ifndef TLA_INPUT_H
#define TLA_INPUT_H

// The Lost Age's key input state: held, pressed and repeating keys.
// ⚓️ のキー入力。☀️ のキー入力は keystate.h にある。

#include "types.h"
#include "layout_guard.h"

struct InputState
{
	u32			 held;
	u32			 pressed;
	u32			 previous_vblank_held;
	/* FAKEMATCH: volatile steers The Lost Age only: without it, in tla-en, WaitFrames: differ 57, Menu_WaitForSelectionInput: 24, Func_02000e4c: 17 and Menu_SelectionLoop: 15. The six tbs editions build the same bytes without it: no tbs code reads gInput. */
	volatile u32 repeat;
	u32			 direction;
	u32			 previous_direction;
	u32			 previous_held;
	u32			 pending_pressed;
	s32			 repeat_timer;
};

LAYOUT_SIZE_GUARD(InputState_Size, struct InputState, 0x24);
LAYOUT_OFFSET_GUARD(InputState_Held, struct InputState, held, 0);
LAYOUT_OFFSET_GUARD(InputState_Pressed, struct InputState, pressed, 4);
LAYOUT_OFFSET_GUARD(InputState_Repeat, struct InputState, repeat, 12);
LAYOUT_OFFSET_GUARD(InputState_RepeatTimer, struct InputState, repeat_timer, 32);

extern struct InputState gInput;
void Input_UpdateKeyRepeatAndDirection(void);

#endif
