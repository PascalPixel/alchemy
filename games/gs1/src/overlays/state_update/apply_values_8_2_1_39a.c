#include "types.h"

#define SceneState_ApplyValues8And2And1 Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02002442(s32, s32, s32);

void SceneState_ApplyValues8And2And1(void) {
    Func_02002442(8, 2, 1);
}
