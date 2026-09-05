#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void SerialRuntime_EnableTransferTimer(void)
{
    s32 state = 0x02002240;
    if (FIELD_AT_OFFSET((void *)state, u8 *, 0) != 0)
        FIELD_AT_OFFSET((void *)state, s8 *, 8) = 1;
}
