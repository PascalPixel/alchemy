#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_02001024();

void Func_02000864(void)
{
    Call2(Func_02001024, 0x17, 0x4d);
}
