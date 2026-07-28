#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08005e70(void)
{
    s32 state = 0x02002240;
    if (M2C_FIELD((void *)state, u8 *, 0) != 0)
        M2C_FIELD((void *)state, s8 *, 8) = 1;
}
