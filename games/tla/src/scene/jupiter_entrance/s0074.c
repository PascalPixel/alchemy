#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_02001b1c();

void Func_02000074(void)
{
    Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x45, 0x16, 1);
}
