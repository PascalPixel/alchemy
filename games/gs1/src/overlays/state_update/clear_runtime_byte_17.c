#include "types.h"

#define SceneState_ClearRuntimeByte17 Func_0200045c
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void SceneState_ClearRuntimeByte17(void) {
    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 0;
}
