#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_020016ca(s32, s32, s32);

void SceneState_ApplyValues14And0And5(void) {
    Func_020016ca(0xE, 0, 5);
}
