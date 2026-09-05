#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_020023f0(s32, s32);

void SceneState_ApplyValues11And62(void) {
    Func_020023f0(0xB, 0x3E);
}
