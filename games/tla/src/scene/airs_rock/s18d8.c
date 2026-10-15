#include "types.h"
#include "iwram_call.h"

s32 Func_020018d8(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a - *b + 0x60000) >> 16;

    return Iwram_Sqrt(dx * dx + dy * dy);
}
