#include "types.h"

#define SceneState_ApplyValues12And2And3 Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02003d96(s32, s32, s32);

void SceneState_ApplyValues12And2And3(void) {
    Func_02003d96(0xC, 2, 3);
}
