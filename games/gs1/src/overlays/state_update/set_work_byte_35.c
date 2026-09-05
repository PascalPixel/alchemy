#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void SceneState_SetWorkByte35(void) {
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}
