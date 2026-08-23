#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Func_08093554(void)
{
    return M2C_FIELD(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0);
}
