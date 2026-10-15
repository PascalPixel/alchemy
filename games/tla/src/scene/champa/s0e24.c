#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 GameFlag_SetBit();

void Func_02000e24(void)
{
    Call1(GameFlag_SetBit, 0x970);
}
