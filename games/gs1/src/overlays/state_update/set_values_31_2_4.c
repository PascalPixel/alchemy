#include "types.h"

#define SceneState_SetValues31_2_4 Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02000e96(s32, s32, s32);

void SceneState_SetValues31_2_4(void) {
    Func_02000e96(0x1F, 2, 4);
}
