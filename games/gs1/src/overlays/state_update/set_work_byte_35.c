#include "types.h"

#define SceneState_SetWorkByte35 Func_02000150
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void SceneState_SetWorkByte35(void) {
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}
