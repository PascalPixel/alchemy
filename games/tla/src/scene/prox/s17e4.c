#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_0200442c();

void Func_020017e4(void)
{
    Call6(Func_0200442c, 0x19, 2, 1, 1, 0x19, 9);
}
