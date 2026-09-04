#include "types.h"

#define SceneState_ApplyArgMode0AndReturnZero Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02000976(s32, s32);

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no) {
    Func_02000976(no, 0);
    return 0;
}
