#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08098cd8();

void BattleFx_CallEffect04(void)
{
    Func_08098cd8();
}
