#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001b44(void)
{
    Func_020019a4(0x57, 2, 0x50, 7, 1, 2);
}
