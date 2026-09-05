#include "types.h"

#define SceneState_SetValue14Mode23 Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02003cd8(s32, s32);

void SceneState_SetValue14Mode23(void) {
    Func_02003cd8(0xE, 0x17);
}
