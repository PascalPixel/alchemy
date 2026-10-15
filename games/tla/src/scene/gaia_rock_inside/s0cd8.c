#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_02004ff4();

void Func_02000cd8(void)
{
    Call7(Func_02004ff4, 0x13, 0x18, 0, 4, 3, 0x1f, 4);
}
