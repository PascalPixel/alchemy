#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void SceneState_SetWorkHalfwordDc(s16 v) {
    FIELD_AT_OFFSET(*(void **)0x03001F3C, s16 *, 0xDC) = v;
}
