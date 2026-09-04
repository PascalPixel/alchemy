#include "types.h"

#define Battle_GetWorkObject1e0 Func_08093554

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Battle_GetWorkObject1e0(void)
{
    return FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0);
}
