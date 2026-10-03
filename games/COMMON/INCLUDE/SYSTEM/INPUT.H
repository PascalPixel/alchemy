#ifndef ALCHEMY_INPUT_H
#define ALCHEMY_INPUT_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct InputState {
    u32 held;
    u32 pressed;
    u32 previous_vblank_held;
    volatile u32 repeat;
    u32 direction;
    u32 previous_direction;
    u32 previous_held;
    u32 pending_pressed;
    s32 repeat_timer;
};

LAYOUT_SIZE_GUARD(InputState_Size, struct InputState, 0x24);
LAYOUT_OFFSET_GUARD(InputState_Held, struct InputState, held, 0);
LAYOUT_OFFSET_GUARD(InputState_Pressed, struct InputState, pressed, 4);
LAYOUT_OFFSET_GUARD(InputState_Repeat, struct InputState, repeat, 12);
LAYOUT_OFFSET_GUARD(InputState_RepeatTimer, struct InputState, repeat_timer, 32);

extern struct InputState gInput;
void Input_UpdateKeyRepeatAndDirection(void);

#endif
